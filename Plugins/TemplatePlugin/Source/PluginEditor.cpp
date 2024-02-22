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

    //startTimerHz(24);
}

NewProjectAudioProcessorEditor::~NewProjectAudioProcessorEditor()
{
    //stopTimer();
}

//==============================================================================
void NewProjectAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    //g.fillAll (juce::Colours::white);

    // First, set the background to desired colour
    g.setColour(juce::Colours::ghostwhite);
    g.fillAll();

    // Now we need a bunch of variables in order to draw the desired plot
    auto threshold = -3.0f;
    auto ratio = 4.0f;

    // Let's start drawing the plot.
    auto y = juce::Decibels::decibelsToGain(threshold) * getHeight();
    auto invertY = (1 - juce::Decibels::decibelsToGain(threshold)) * getHeight();

    g.setColour(juce::Colours::darkred);
    g.drawLine(0, getHeight(), y, invertY, 2);

    g.drawLine(y, invertY, getWidth(), invertY - (invertY / ratio), 2);

    /// TODO: Implement knee


}

void NewProjectAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void NewProjectAudioProcessorEditor::timerCallback()
{
}
