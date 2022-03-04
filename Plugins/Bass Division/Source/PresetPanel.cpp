/*
  ==============================================================================

    PresetPanel.cpp
    Created: 28 Aug 2020 5:31:53pm
    Author:  Jose Diaz

  ==============================================================================
*/

#include "PresetPanel.h"

//#include "../../Plugins/BassDivision/Source/PluginProcessor.h"
//#include "VPresetManager.h"

/*
    reminder to check whether the pointers you are getting to the presetManager are valid
    before using!
 */

PresetPanel::PresetPanel(BassDivisionAudioProcessor& p): audioProcessor(p)
{
	//setSize(525, 30);
	
    // set up save and load buttons
	savePreset.setButtonStyle(DivisionVoid::Button::ButtonStyle::Bar);
	savePreset.setButtonText("Save");
	addAndMakeVisible(savePreset);
    
    savePreset.onClick = [this] ()
    {
        // get a pointer to our preset manager
        auto presetManager = audioProcessor.getPresetManager();

        // create a String holding the current preset's path
        juce::String currentPresetPath = presetManager->getCurrentPresetDirectory() + directorySeperator + presetManager->getCurrentPresetName();
        currentPresetPath.removeCharacters(juce::StringRef("*"));
               
        // use a FileChooser to let the user select the save file
        // default will be currently chosen file
        juce::FileChooser chooser ("Save a file: ", juce::File(currentPresetPath), presetFileExtensionWildcard);
               
        // if the user has chosen a file, get the result and pass it to
        // presetManager savePreset() and update the comboBox
        if (chooser.browseForFileToSave(true))
        {
            juce::File presetToSave(chooser.getResult());
            
            presetManager->savePreset(presetToSave);
                   
            updatePresetComboBox();
        }
    };

    // set up preset combo box
	presetComboBox.setColour(juce::ComboBox::ColourIds::backgroundColourId, juce::Colours::black);
	presetComboBox.setColour(juce::ComboBox::ColourIds::textColourId, juce::Colours::darkgrey);
	presetComboBox.setColour(juce::ComboBox::ColourIds::outlineColourId, juce::Colours::grey);

	addAndMakeVisible(presetComboBox);
	presetComboBox.setOpaque(false);
	presetComboBox.setLookAndFeel(&lookAndFeel);
	presetComboBox.setText(currentPresetName, juce::dontSendNotification);

    // adds preset items to combo box
	updatePresetComboBox();
    
    presetComboBox.onChange = [this] ()
    {
        // check to see if an item has actually been selected
        if (presetComboBox.getSelectedItemIndex() != -1)
        {
            // if so, get a pointer to the preset manager
            auto presetManager = audioProcessor.getPresetManager();
            const int index = presetComboBox.getSelectedItemIndex();

            // then create a File object using the preset directory
            // and the preset name correstponding to the combo box object
            // pass that File into the presetLoad() method
            auto presetToLoad = juce::File(presetManager->getCurrentPresetDirectory() + directorySeperator /*"\\"*/ + presetManager->getPresetName(index) + presetFileExtension);

            // Perhaps you could just use a get file by index method???

            presetManager->loadPreset(presetToLoad);
        }
    };
    
    // start timer
	startTimerHz(20);
}

PresetPanel::~PresetPanel()
{
    // stop timer
	stopTimer();
}

void PresetPanel::paint(juce::Graphics& g)
{
	
	g.fillAll(juce::Colours::whitesmoke.withAlpha(0.f));
}

void PresetPanel::timerCallback()
{
    // get pointer to preset manager
	auto presetManager = audioProcessor.getPresetManager();

    // check to see if the locally held current preset name is different
    // than that held by preset manager. if so, change the locally held
    // current preset name to that held by the preset manager and update
    // the combobox

	auto presetNameInManager = presetManager->getCurrentPresetName();
    if (presetNameInManager != currentPresetName)
    {
		presetComboBox.setText(presetNameInManager, juce::dontSendNotification);
		currentPresetName = presetNameInManager;
    }
    
    /*
        save the currently loaded preset name as part of the processor state
        by adding it as a property to the APVTS used to save in getStateInformation().
     
        then, in setStateInformation, check if that property is there—if so, update
        the currentPresetName in Manager
     */

	//if (presetManager->getStateHasChanged())
		//presetDisplayName = currentPresetName + "*";
}

void PresetPanel::updatePresetComboBox()
{
    // get pointer to preset manager
	auto presetManager = audioProcessor.getPresetManager();
	
    // clear all items from the combo box
	presetComboBox.clear(juce::dontSendNotification);
    
    // get the number of presets, and use them to iterate through the preset list,
    // adding items to the presetDisplay as we go
	for (auto i = 0; i < presetManager->getNumberOfPresets(); i++)
	{
		presetComboBox.addItem(presetManager->getPresetName(i), (i + 1));
	}

    // set the presetComboBox's text to the currently selected preset
	presetComboBox.setText(presetManager->getCurrentPresetName());
}

void PresetPanel::resized()
{
	auto area = getLocalBounds();
	auto border = 4;

	auto buttonArea = area.removeFromLeft(area.getWidth() * .33);

	savePreset.setBounds(buttonArea.removeFromLeft(buttonArea.getWidth()).reduced(border));
	//loadPreset.setBounds(buttonArea.reduced(border));
	presetComboBox.setBounds(area.reduced(border));

	savePreset.setFont(DVFonts::getBoldNorse(15));
}
