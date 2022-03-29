/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

#include "GUI/MainPanel.h"


//==============================================================================
/**
*/
class BassDivisionAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    BassDivisionAudioProcessorEditor (BassDivisionAudioProcessor&);
    ~BassDivisionAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:

    MainPanel mMainPanel;
    
    BassDivisionAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BassDivisionAudioProcessorEditor)
};
