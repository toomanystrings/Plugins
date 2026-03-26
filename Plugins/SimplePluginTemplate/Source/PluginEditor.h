
#pragma once

#include "PluginProcessor.h"
#include "UI/PanelBase.h"

class NewPluginTemplateAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    explicit NewPluginTemplateAudioProcessorEditor(NewPluginTemplateAudioProcessor&);

private:
    void paint(juce::Graphics&) override;
    void resized() override;

    Panel panel;

    NewPluginTemplateAudioProcessor& mProcessor;
};
