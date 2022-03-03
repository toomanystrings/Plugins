/*
  ==============================================================================

    PresetPanel.h
    Created: 28 Aug 2020 5:31:53pm
    Author:  Jose Diaz

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "SynthLookAndFeel.h"


class PresetPanel  : public juce::Component,
                     public juce::Timer
{
public:
    
    // replace with name of your audio processor
    PresetPanel (BassDivisionAudioProcessor&);
    ~PresetPanel();
    
    void paint (juce::Graphics& g) override;
    
    void resized() override;
            
    void timerCallback() override;
    
    void updatePresetComboBox();
    
private:
	
	DivisionVoid::Button savePreset;
    //juce::TextButton savePreset;
    juce::TextButton loadPreset;

	//DivisionVoid::Selector presetComboBox;
	juce::ComboBox presetComboBox;

    juce::String currentPresetName {"Factory Settings"};
	juce::String presetDisplayName { "Factory Settings" };

	SynthLookAndFeel lookAndFeel;

	Utilities utils;
	//juce::Font norse = utils.getNorseBoldFont();
    
	BassDivisionAudioProcessor& audioProcessor;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PresetPanel);
};
