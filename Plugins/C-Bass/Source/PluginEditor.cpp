
#include "PluginProcessor.h"
#include "PluginEditor.h"

CBassAudioProcessorEditor::CBassAudioProcessorEditor(CBassAudioProcessor& p)
        : juce::AudioProcessorEditor(&p),
          mProcessor(p)
{
    setSize(1000, 600);

    // The following is pure vibes. Delete at the end
    seabass = juce::ImageFileFormat::loadFrom(
            juce::File("/Users/conorforan/Documents/Plugins - mega repo/Plugins/C-Bass/Source/UI/seabass.jpg"));
    if (seabass.isValid())
        imageComponent.setImage(seabass);

    addAndMakeVisible(&imageComponent);
}

void CBassAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void CBassAudioProcessorEditor::resized()
{
    // Delete at the end.
    imageComponent.setBounds(getBounds());
}