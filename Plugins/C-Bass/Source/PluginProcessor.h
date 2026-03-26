
#pragma once

#include <JuceHeader.h>

class CBassAudioProcessor : public DivisionVoid::ProcessorBase, juce::AudioProcessorValueTreeState::Listener
{
public:
    CBassAudioProcessor ();

    void processBlock (juce::AudioBuffer<float> &, juce::MidiBuffer &) override;
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;

    juce::AudioProcessorEditor *createEditor () override;

    void getStateInformation (juce::MemoryBlock &destData) override;

    void setStateInformation (const void *data, int sizeInBytes) override;

private:
    // Parameters
    juce::AudioProcessorValueTreeState apvts;
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    void parameterChanged(const juce::String& parameterID, float newValue) override;

};