//
// Created by Conor Foran on 10/03/2022.
//

#pragma once

#include <iostream>
#include <iomanip>
#include <memory>

#if JUCE_WINDOWS
static const juce::String directorySeperator = "\\";

#elif JUCE_MAC
static const juce::String directorySeperator = "/";
#endif

#define ANIMATION_SPEED 0.1
#define ANIMATION_FPS 60

#define CORNER_RADIUS   14
#define CORNER_CONFIG   CORNER_RADIUS, CORNER_RADIUS, false, true, true, false

#define ROTARY_ANGLESTART ((7.0f * M_PI) / 6.0f)
#define ROTARY_ANGLERANGE ((5.0f * M_PI) / 3.0f)

#define DEFAULT_PIPMIN 6
#define DEFAULT_PIPMAX 8

#define MOUSEWHEEL_SENSITIVITY 128

constexpr double pi    = 3.141592653589793238462643383279;
constexpr double twoPi = 2.0 * pi;

// Little helpers to reduce the amount of typing for smart pointers
template <typename t>
using UniquePtr = std::unique_ptr<t>;

template<typename T, typename... Args>
UniquePtr<T> MakeUnique(Args&&... args)
{
    return std::make_unique<T>(std::forward<Args>(args)...);
}

inline double getScaledValueFromNormalizedValue(double input, double lowerBound, double upperBound, double skew = 1.0)
{
    jassert(input >= 0.0 && input <= 1.0);

    double skewedInput = std::pow(input, skew);

    return (skewedInput * (lowerBound - upperBound)) + upperBound;
}

inline double getNormalizedValueFromScaledRange(double input, double lowerBound, double upperBound, double skew = 1.0)
{
    double output = (input - lowerBound) / (upperBound - lowerBound);

    return pow(output, 1.0 / skew);
}

inline float logTransformInRange0to1 (const float between0and1)
{
    const float minimum = 1.0f;
    const float maximum = JUCE_LIVE_CONSTANT (1000.0f);
    // return drow::logBase10Scale (frequency, 1.0f, maximum);
    return log10 (minimum + ((maximum - minimum) * between0and1)) / log10 (maximum);
}

inline float expTransformInRange0to1 (const float between0and1)
{
    const float minimum = 1.0f;
    const float maximum = JUCE_LIVE_CONSTANT (1000.0f);
    return (pow(maximum, between0and1) - minimum) / (maximum - minimum);
}

class AbstractWriter
{
public:
    AbstractWriter() = default;

    void reset(int capacity, int samplesToRead) noexcept
    {
        jassert(capacity > 0);
        jassert(samplesToRead > 0 && samplesToRead < capacity);
        bufferSize = capacity;
        readSize = samplesToRead;
        writePos = 0;
        writePos.store(0);
    }

    void prepareToWrite(int numToWrite, int& startIndex1, int& blockSize1, int& startIndex2, int& blockSize2) const noexcept
    {
        const int ve = writePos.load();
        const int localReadSize = readSize.load();

        numToWrite = juce::jmin(numToWrite, bufferSize - localReadSize);

        if (numToWrite <= 0)
        {
            startIndex1 = 0;
            startIndex2 = 0;
            blockSize1 = 0;
            blockSize2 = 0;
        }
        else
        {
            startIndex1 = ve;
            startIndex2 = 0;
            blockSize1 = juce::jmin(bufferSize - ve, numToWrite);
            numToWrite -= blockSize1;
            blockSize2 = numToWrite <= 0 ? 0 : juce::jmin(numToWrite, bufferSize - localReadSize);
        }
    }

    void finishedWrite(int numWritten) noexcept
    {
        jassert(numWritten >= 0 && numWritten < bufferSize);

        int newEnd = writePos.load() + numWritten;

        if (newEnd >= bufferSize)
            newEnd -= bufferSize;

        writePos.store(newEnd);
    }

    void prepareToRead(int& startIndex1, int& blockSize1, int& startIndex2, int& blockSize2) const noexcept
    {
        const int localReadSize = readSize.load();

        int vs = writePos.load() - localReadSize;
        if (vs < 0) vs += bufferSize;

        int numWanted = localReadSize;

        startIndex1 = vs;
        startIndex2 = 0;
        blockSize1 = juce::jmin(bufferSize - vs, localReadSize);
        numWanted -= blockSize1;
        blockSize2 = numWanted <= 0 ? 0 : numWanted;
    }

    void setReadSize(int newValue) noexcept
    {
        jassert(newValue > 0 && newValue < bufferSize);
        readSize.store(newValue);
    }
private:
    int bufferSize;

    std::atomic <int> readSize;
    std::atomic <int> writePos;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AbstractWriter)
};

class Fifo {
public:
    void reset(int numberOfChannels, int capacity)
    {
        fifo.reset(capacity, capacity - 1);
        buffer.clear();
        buffer.setSize(numberOfChannels, capacity, true, false, true);
    }

    void push(const juce::AudioBuffer<float>& data)
    {
        const auto numberOfItems = data.getNumSamples();
        jassert(numberOfItems < buffer.getNumSamples());
        jassert(data.getNumChannels() == buffer.getNumChannels());

        int start1, size1, start2, size2;
        fifo.prepareToWrite(numberOfItems, start1, size1, start2, size2);

        if (size1 > 0)
            for (auto channel = 0; channel < buffer.getNumChannels(); channel++)
                buffer.copyFrom(channel, start1, data, channel, 0, size1);
        if (size2 > 0)
            for (auto channel = 0; channel < buffer.getNumChannels(); channel++)
                buffer.copyFrom(channel, start2, data, channel, size1, size2);
        fifo.finishedWrite(size1 + size2);
    }

    void pull(juce::AudioBuffer<float>& data, int numberOfItems)
    {
        jassert(numberOfItems < buffer.getNumSamples());
        jassert(data.getNumChannels() == buffer.getNumChannels());

        int start1, size1, start2, size2;
        fifo.setReadSize(numberOfItems);
        fifo.prepareToRead(start1, size1, start2, size2);

        if (size1 > 0)
            for (auto channel = 0; channel < buffer.getNumChannels(); channel++)
                data.copyFrom(channel, 0, buffer, channel, start1, size1);
        if (size2 > 0)
            for (auto channel = 0; channel < buffer.getNumChannels(); channel++)
                data.copyFrom(channel, size1, buffer, channel, start2, size2);
    }

    void pull(float* destination, int channel, int numberOfItems)
    {
        jassert(numberOfItems < buffer.getNumSamples());
        jassert(channel >= 0 && channel < buffer.getNumChannels());

        int start1, size1, start2, size2;
        fifo.setReadSize(numberOfItems);
        fifo.prepareToRead(start1, size1, start2, size2);

        const auto* source = buffer.getReadPointer(channel);
        if (size1 > 0)
            std::copy_n(source + start1, size1, destination);
        if (size2 > 0)
            std::copy_n(source + start2, size2, destination + size1);
    }
private:
    AbstractWriter fifo;
    juce::AudioBuffer<float> buffer;
};
