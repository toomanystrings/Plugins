
#pragma once

#include "PluginProcessor.h"

class CBassAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    explicit CBassAudioProcessorEditor(CBassAudioProcessor&);

private:
    void paint(juce::Graphics&) override;
    void resized() override;

    juce::Image seabass;
    juce::ImageComponent imageComponent;

    CBassAudioProcessor& mProcessor;
};
