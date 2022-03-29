/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BassDivisionAudioProcessorEditor::BassDivisionAudioProcessorEditor (BassDivisionAudioProcessor& p)
    : AudioProcessorEditor (&p), /*mMainPanel(p)*/ audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    //setSize (MAIN_PANEL_WIDTH, MAIN_PANEL_HEIGHT - 100);
    setSize(800, 600);
    //addAndMakeVisible (mMainPanel);
}

BassDivisionAudioProcessorEditor::~BassDivisionAudioProcessorEditor()
{
}

//==============================================================================
void BassDivisionAudioProcessorEditor::paint (juce::Graphics& g)
{
}

void BassDivisionAudioProcessorEditor::resized()
{
    auto area = getLocalBounds();
    
    //mMainPanel.setBounds(area);
}
