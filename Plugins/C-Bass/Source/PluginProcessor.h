
#pragma once

#include "Parameters.h"
#include <JuceHeader.h>

class CBassAudioProcessor : public DivisionVoid::ProcessorBase
{
public:
    CBassAudioProcessor ();

    void processBlock (juce::AudioBuffer<float> &, juce::MidiBuffer &) override;

    juce::AudioProcessorEditor *createEditor () override;

    void getStateInformation (juce::MemoryBlock &destData) override;

    void setStateInformation (const void *data, int sizeInBytes) override;

private:

    Parameters parameters;
};