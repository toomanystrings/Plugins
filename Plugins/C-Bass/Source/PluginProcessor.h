
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

    float gain = 1.0f;
    float intensity = 1.0f;

    // Filters
    using Filter = juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>>;
    Filter bandpassFilter, lowpassFilter, highpassFilter;

    //Oversampling
    juce::dsp::Oversampling<float> os;
    size_t osFactor = 3;

    juce::AudioBuffer<float> bandBuffer, tempBuffer;
};