//
// Created by Conor Foran on 17/11/2025.
//

#pragma once

namespace DivisionVoid
{
    class BeatSync
    {
    public:
        BeatSync () {}
        ~BeatSync() = default;

        void getTempo (juce::AudioPlayHead* playHead)
        {
            if (juce::JUCEApplicationBase::isStandaloneApp() || playHead == nullptr)
                return;
            const auto playHeadPosition = playHead->getPosition();
            if (!playHeadPosition.hasValue())
                return;
            const auto& posInfo = *playHeadPosition;

            tempo = static_cast<double>(*posInfo.getBpm());
        }

        void prepareToPlay(double sampleRate, int samplesPerBlock)
        {
            this->SampleRate = sampleRate;
            this->bufferSize = samplesPerBlock;
        }



    private:

        double tempo;

        double SampleRate;
        int bufferSize;

    };
}
