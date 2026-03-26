
#include "PluginProcessor.h"
#include "PluginEditor.h"

NewPluginTemplateAudioProcessorEditor::NewPluginTemplateAudioProcessorEditor(
        NewPluginTemplateAudioProcessor& p)
        : juce::AudioProcessorEditor(&p), mProcessor(p), panel(p)
{
    addAndMakeVisible(panel);
    setSize(1000, 600);
}

void NewPluginTemplateAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void NewPluginTemplateAudioProcessorEditor::resized()
{
    panel.setBounds(this->getBounds());
}