//
// Created by Conor Foran on 17/09/2023.
//

#ifndef JUCECMAKEREPO_DELAYPROCESSOR_H
#define JUCECMAKEREPO_DELAYPROCESSOR_H

namespace AdtDSP
{
    inline float msToSamples(float sampleRate, int ms)
    {
        return (ms / 1000) * sampleRate;
    }

    inline float samplesToMs(float sampleRate, int samples)
    {
        return samples / sampleRate * 1000;
    }

    struct DelayProcessor
    {
    public:
        void prepare(float sampleRate, int bufferSize, int numChannels)
        {
            juce::dsp::ProcessSpec spec;

            spec.sampleRate = sampleRate;
            spec.maximumBlockSize = bufferSize;
            spec.numChannels = numChannels;

            delayLine.reset();
            delayLine.prepare(spec);
        }

        void process(juce::AudioBuffer<float>& buffer)
        {

        }

    private:

        juce::dsp::DelayLine<float, juce::dsp::DelayLineInterpolationTypes::Linear> delayLine;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayProcessor)
    };
}


#endif //JUCECMAKEREPO_DELAYPROCESSOR_H
