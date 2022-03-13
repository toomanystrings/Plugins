/*
  ==============================================================================

    PresetManager.h
    Created: 28 Aug 2020 5:28:22pm
    Author:  Jose Diaz

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../Utilities.h"

// File extension is customisable. Can realistically be whatever you want
static const juce::String presetFileExtension = ".tbp";
static const juce::String presetFileExtensionWildcard = "*.tbp";

class PresetManager
{
public:
    
    PresetManager(juce::AudioProcessor* p);

    /** Sets all parameters to default and sets preset name to "Untitled" */
    void createNewPreset();
    
    /**  Takes a user selected file and saves the APVTS state to it */
    void savePreset (juce::File presetToSave);
    
    /**  Takes a user selected file and loads data from it into the APVTS */
    void loadPreset (juce::File presetToLoad);

    /** Returns the present name for a given index of the preset directory */
    const juce::String getPresetName (int inPresetIndex);
    
    /** Returns the name of the currently loaded preset */
    const juce::String getCurrentPresetName();
    
    /** Returns the base Preset Directory */
    const juce::String getCurrentPresetDirectory();
    
    /** Allows caller to set the name of the currently loaded preset. used to facilitate state restore */
    void setCurrentPresetName (juce::String newPresetName);
    
    /** Returns the number of presets loaded to the local presets array */
    int getNumberOfPresets();

	//void setStateHasChanged() { parameterHasChanged = true; }
	//bool getStateHasChanged() { return parameterHasChanged; }
 
private:
	
	
    /** Iterates over the preset directory and adds the files to localPresets  */
    void updatePresetList();

    juce::File presetDirectory;

    juce::String currentPresetName {"Factory Settings"};

    juce::Array<juce::File> presetList;

	//bool parameterHasChanged = false;

	juce::AudioProcessor* audioProcessor;
};
