/*
  ==============================================================================

    MeterAnalysis.cpp
    Created: 9 Jun 2021 5:08:35pm
    Author:  Olorin

  ==============================================================================
*/

#include "MeterAnalysis.h"

MeterAnalysis::MeterAnalysis(int size) :
									fifo(size, true),
									RMSValue(juce::var(0.0f)),
									repaintViewer(juce::var(false)),
                                    isPlaying(juce::var(false)),
                                    needToProcess(false)
{
}

MeterAnalysis::~MeterAnalysis()
{
}

void MeterAnalysis::setSampleRate(double sampleRate)
{
    if (this->sampleRate != sampleRate) 
    {
        this->sampleRate = sampleRate;
        // Update Rise and Fall Time, these values were picked to match Logic's meter
        ga = exp(-log(9) / (sampleRate * 0.01));
        gr = exp(-log(9) / (sampleRate * 0.5));

        previousSample[0] = 0.0f;
        previousSample[1] = 0.0f;
    }
}

void MeterAnalysis::copySamples(std::vector<float>& array, bool isPlaying)
{
    this->isPlaying = isPlaying;

    if (isPlaying)
    {
        fifo.addValuesToFifo(array.data(), static_cast<int>(array.size()));
    }
        

    needToProcess = true;
}

int MeterAnalysis::useTimeSlice()
{
    if (needToProcess)
    {
        process();
        needToProcess = false;
    }

    const int sleepTime = 5; // [ms]
    return sleepTime;
}

void MeterAnalysis::process()
{
    int fifoSize = fifo.getFifoSize();
    float total = 0.0f;
    for (int i = 0; i < fifoSize; ++i)
        total += fifo.getValueAt(i);

    float rms = sqrtf(total / static_cast<float>(fifoSize));

    if (previousSample[0] < rms) {
        g = ga; // Meter rising
    }
    else {
        g = gr; // Meter falling
    }

    float outputSample = (1.0f - g) * rms + g * previousSample[0];
    previousSample[0] = outputSample;

    // Convert to decibel scale
    outputSample = 20.0f * log10(outputSample / JUCE_LIVE_CONSTANT(0.707f));

    // Set a floor, -75 was picked to closely match the meter in Logic
    if (outputSample < -75.0f) { outputSample = -75.0f; }

    // Convert from scale: -75 dB to 0 db, over to the scale: 0 to 1 for the meter
    outputSample = (outputSample / 75.0f) + 1;

    RMSValue = outputSample;

    repaintViewer = true;
}

juce::Value& MeterAnalysis::getRMSValue()
{
    return RMSValue;
}

juce::Value& MeterAnalysis::getRepaintViewer()
{
    return repaintViewer;
}

juce::Value& MeterAnalysis::getIsPlaying()
{
    return isPlaying;
}
