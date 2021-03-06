/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
NewProjectAudioProcessorEditor::NewProjectAudioProcessorEditor (NewProjectAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (500, 500);
    addAndMakeVisible(visualiser);

    //audioMeter.setOrientation(DivisionVoid::SegmentedDisplayMeter::Orientation::Horizontal);
    //addAndMakeVisible(audioMeter);
}

NewProjectAudioProcessorEditor::~NewProjectAudioProcessorEditor() = default;

//==============================================================================
void NewProjectAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    //g.fillAll (juce::Colours::white);

    //g.setColour (juce::Colours::white);
    //g.setFont (fonts.getFont(DivisionVoidFonts::FontType::bold, 15.0f));
    //g.drawFittedText ("Template Plugin", getLocalBounds(), juce::Justification::centred, 1);
}

void NewProjectAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    //audioMeter.setOrientation(DivisionVoid::SegmentedDisplayMeter::Orientation::Horizontal);

    //audioMeter.setBounds(50, 50, 400, 25);

    visualiser.setBounds(getLocalBounds());
}
