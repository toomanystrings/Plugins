//
// Created by Conor Foran on 12/01/2023.
//

#pragma once

#include "../Utilities.h"

namespace DivisionVoid
{
    class LevelMeterProcessor
    {
    public:
        LevelMeterProcessor() = default;
        ~LevelMeterProcessor() = default;

        void prepare(float sampleRate, int samplesPerBlock, int numChannels)
        {
            fs = sampleRate;

            rmsLevels.clear();
            for (auto i = 0; i < numChannels; ++i)
            {
                juce::LinearSmoothedValue<float> rms {-100.0f};
                rms.reset(fs, 0.5);
                rmsLevels.emplace_back(std::move(rms));
            }

            rmsFifo.reset(numChannels, static_cast<int>(sampleRate) + 1);
            rmsCalculationBuffer.clear();
            rmsCalculationBuffer.setSize(numChannels, static_cast<int>(sampleRate) + 1);
        }

        std::vector<float> getRmsLevels()
        {
            rmsFifo.pull(rmsCalculationBuffer, rmsWindowSize);
            std::vector<float> levels;
            for (auto channel = 0; channel < rmsCalculationBuffer.getNumChannels(); channel++)
            {
                processLevelValue(rmsLevels[channel], juce::Decibels::gainToDecibels(rmsCalculationBuffer.getRMSLevel(channel, 0, rmsWindowSize)));
                levels.push_back(rmsLevels[channel].getCurrentValue());
            }
            return levels;
        }

        float getRmsLevel(const int channel)
        {
            jassert(channel >= 0 && channel < rmsCalculationBuffer.getNumChannels());
            rmsFifo.pull(rmsCalculationBuffer.getWritePointer(channel), channel, rmsWindowSize);
            processLevelValue(rmsLevels[channel], juce::Decibels::gainToDecibels(rmsCalculationBuffer.getRMSLevel(channel, 0, rmsWindowSize)));
            return rmsLevels[channel].getCurrentValue();
        }

        void pushValues(juce::AudioBuffer<float> buffer)
        {
            rmsFifo.push(buffer);
        }

    private:

        void processLevelValue(juce::LinearSmoothedValue<float>& smoothedValue, const float value) const
        {
            if (isSmoothed)
            {
                if (value < smoothedValue.getCurrentValue())
                {
                    smoothedValue.setTargetValue(value);
                    return;
                }
            }
            smoothedValue.setCurrentAndTargetValue(value);
        }

        juce::LinearSmoothedValue<float> gain;
        std::vector<juce::LinearSmoothedValue<float>> rmsLevels;
        juce::AudioBuffer<float> rmsCalculationBuffer;

        Fifo rmsFifo;

        int rmsWindowSize = 250;
        double fs = 44100.0;
        bool isSmoothed = true;
    };
}
