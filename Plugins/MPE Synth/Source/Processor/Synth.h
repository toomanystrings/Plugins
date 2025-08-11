//
// Created by Conor Foran on 01/01/2025.
//

#pragma once

#include <JuceHeader.h>

class SynthFace : public juce::MPESynthesiserVoice
{
public:
    SynthFace() = default;

    void noteStarted() override {}
    void noteStopped(bool allowTailOff) override {}

    void notePressureChanged() override {}
    void notePitchbendChanged() override {}
    void noteTimbreChanged() override {}
    void noteKeyStateChanged() override {}

    void setCurrentSampleRate(double newRate) override {}

    void renderNextBlock (juce::AudioBuffer<float>& outputBuffer,
                          int startSample,
                          int numSamples) override {}

private:
    //float getNextSample() noexcept {}

    // We will need params here.

};