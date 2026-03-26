
#include "PluginProcessor.h"
#include "PluginEditor.h"

CBassAudioProcessorEditor::CBassAudioProcessorEditor(
        CBassAudioProcessor& p)
        : juce::AudioProcessorEditor(&p), mProcessor(p)
{
    setSize(1000, 600);
}

void CBassAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void CBassAudioProcessorEditor::resized()
{

}