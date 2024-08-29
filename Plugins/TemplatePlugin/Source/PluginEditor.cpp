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
    setSize (600, 600);

    addAndMakeVisible(slider);

    attachments[0] = MakeUnique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            audioProcessor.treeState, "INNER", slider.getInnerSlider());

    attachments[1] = MakeUnique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            audioProcessor.treeState, "OUTER", slider.getOuterSlider());

    std::vector<float> outerPips = {-64, -48, -32, -24, -12, 0, 12, 24, 32, 48, 64};
    std::vector<float> innerPips = {-1, -0.9, -0.8, -0.7, -0.6, -0.5, -0.4, -0.3, -0.2, -0.1, 0,
                                    0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1};

    slider.setOuterTickValues(outerPips);
    slider.setInnerTickValues(innerPips);
}

NewProjectAudioProcessorEditor::~NewProjectAudioProcessorEditor()
{
}

//==============================================================================
void NewProjectAudioProcessorEditor::paint (juce::Graphics& g)
{


}

void NewProjectAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor...

    auto localArea = getBounds();
    //localArea.setCentre(getWidth() / 2, getHeight() / 2);

    slider.setBounds(localArea);
}
