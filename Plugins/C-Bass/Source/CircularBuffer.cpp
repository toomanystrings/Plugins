//
// Created by Conor Foran on 05/06/2026.
//

#include "CircularBuffer.h"

void CircularBuffer::prepare(int sizeInSamples)
{
    buffer.assign(sizeInSamples, 0.0f);
    writeIndex = 0;
    bufferSize = sizeInSamples;
}

void CircularBuffer::write(float sample)
{
    buffer[writeIndex] = sample;
    writeIndex = (writeIndex + 1) % bufferSize;
}

float CircularBuffer::readDelayed(int delaySamples) const
{
    delaySamples = juce::jlimit(0, bufferSize - 1, delaySamples);
    int readIndex = (writeIndex - delaySamples - 1 + bufferSize) % bufferSize;
    return buffer[readIndex];
}

float CircularBuffer::readDelayedInterpolated(float delaySamples) const
{
    delaySamples = juce::jlimit(0.0f, static_cast<float>(bufferSize - 2), delaySamples);
    int   intPart  = static_cast<int>(delaySamples);
    float fracPart = delaySamples - static_cast<float>(intPart);
    float sampleA  = readDelayed(intPart);
    float sampleB  = readDelayed(intPart + 1);
    return sampleA + fracPart * (sampleB - sampleA);
}

// Mean absolute amplitude of the last nSamples — our "SampleSum" 0..1.
float CircularBuffer::getAverageAmplitude(int nSamples) const
{
    nSamples = juce::jlimit(1, bufferSize, nSamples);
    float sum = 0.0f;
    for (int i = 0; i < nSamples; ++i)
    {
        int idx = (writeIndex - i - 1 + bufferSize) % bufferSize;
        sum += std::abs(buffer[idx]);
    }
    return sum / static_cast<float>(nSamples);
}

int CircularBuffer::getSize() const { return bufferSize; }
