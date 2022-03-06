/*
  ==============================================================================

    IrPanel.cpp
    Created: 12 Nov 2020 6:46:47pm
    Author:  Olorin

  ==============================================================================
*/

#include <JuceHeader.h>
#include "IrPanel.h"
#include "Utilities.h"

//==============================================================================
ImpulseResponsePanel::ImpulseResponsePanel(BassDivisionAudioProcessor& p): audioProcessor(p)
{
	//setSize(525, 30);

	// set up save and load buttons
	loadImpulseResponse.setButtonStyle(DivisionVoid::Button::ButtonStyle::Bar);
	loadImpulseResponse.setButtonText("Load IR");
	addAndMakeVisible(loadImpulseResponse);
    
    loadImpulseResponse.onClick = [this] ()
    {
        // get a pointer to our preset manager
        auto impulseResponseManager = audioProcessor.getImpulseResponseManager();

        // get the current directory name
        juce::String currentPresetDirectory = impulseResponseManager->getCurrentImpulseResponseDirectory();

        // if the preset directory is valid, create a fileChooser to let
        // user select a file to load
        if (currentPresetDirectory.isNotEmpty())
        {
            juce::FileChooser chooser("Load an IR: ", juce::File(currentPresetDirectory), "*.wav");

            // if user selects a file take the result and pass it to preset manager
            // loadPreset method
            if (chooser.browseForFileToOpen())
            {
                juce::File impulseResponseToLoad(chooser.getResult());
                impulseResponseManager->loadImpulseResponse(impulseResponseToLoad);
                engageImpulseResponse.setToggleState(true, juce::NotificationType::dontSendNotification);
            }
        }
    };

	// set up save and load buttons
	engageImpulseResponse.setButtonStyle(DivisionVoid::Button::ButtonStyle::BarToggle);
    engageImpulseResponse.setButtonText("IR Enabled");
    engageIRAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, "IR_ENABLED", engageImpulseResponse);
	addAndMakeVisible(engageImpulseResponse);

	// set up preset combo box
	impulseResponseComboBox.setColour(juce::ComboBox::ColourIds::backgroundColourId, juce::Colours::black);
	impulseResponseComboBox.setColour(juce::ComboBox::ColourIds::textColourId, juce::Colours::darkgrey);
	impulseResponseComboBox.setColour(juce::ComboBox::ColourIds::outlineColourId, juce::Colours::grey);

	addAndMakeVisible(impulseResponseComboBox);
	impulseResponseComboBox.setOpaque(false);
	impulseResponseComboBox.setLookAndFeel(&lookAndFeel);
	
	impulseResponseComboBox.setText(currentImpulseResponseName, juce::dontSendNotification);
    
    impulseResponseComboBox.onChange = [this] ()
    {
        // check to see if an item has actually been selected
        if (impulseResponseComboBox.getSelectedItemIndex() != -1)
        {
            // if so, get a pointer to the preset manager
            auto impulseResponseManager = audioProcessor.getImpulseResponseManager();
            const int index = impulseResponseComboBox.getSelectedItemIndex();

            // then create a File object using the preset directory
            // and the preset name correstponding to the combo box object
            // pass that File into the presetLoad() method
            auto irToLoad = juce::File(impulseResponseManager->getCurrentImpulseResponseDirectory()
                                            + directorySeperator /*"\\"*/ + impulseResponseManager->getImpulseResponseName(index) + ".wav");

            impulseResponseManager->loadImpulseResponse(irToLoad);
        }
    };

	// adds preset items to combo box
	updateImpulseResponseComboBox();

	// start timer
	startTimerHz(20);
}

ImpulseResponsePanel::~ImpulseResponsePanel()
{
	stopTimer();
}

void ImpulseResponsePanel::paint (juce::Graphics& g)
{
	g.fillAll(juce::Colours::whitesmoke.withAlpha(0.f));
}

void ImpulseResponsePanel::resized()
{
	auto area = getLocalBounds();
	auto border = 4;

	auto buttonArea = area.removeFromLeft(area.getWidth() * .5);

	loadImpulseResponse.setBounds(buttonArea.removeFromLeft(buttonArea.getWidth() / 2).reduced(border));
    engageImpulseResponse.setBounds(buttonArea.removeFromLeft(buttonArea.getWidth()).reduced(border));
	impulseResponseComboBox.setBounds(area.reduced(border));

	loadImpulseResponse.setFont(DVFonts::getBoldNorse(15));
    engageImpulseResponse.setFont(DVFonts::getBoldNorse(15));
}

void ImpulseResponsePanel::timerCallback()
{
	// get pointer to preset manager
	auto impulseResponseManager = audioProcessor.getImpulseResponseManager();

	// check to see if the locally held current preset name is different
	// than that held by preset manager. if so, change the locally held
	// current preset name to that held by the preset manager and update
	// the combobox

	auto presetNameInManager = impulseResponseManager->getCurrentImpulseResponseName();
	if (presetNameInManager != currentImpulseResponseName)
	{
		impulseResponseComboBox.setText(presetNameInManager, juce::dontSendNotification);
		currentImpulseResponseName = presetNameInManager;
    }
}

void ImpulseResponsePanel::updateImpulseResponseComboBox()
{
	// get pointer to preset manager
	auto impulseResponseManager = audioProcessor.getImpulseResponseManager();

	// clear all items from the combo box
	impulseResponseComboBox.clear(juce::dontSendNotification);

	// get the number of presets, and use them to iterate through the preset list,
	// adding items to the presetDisplay as we go
	for (auto i = 0; i < impulseResponseManager->getNumberOfImpulseResponses(); i++)
	{
		impulseResponseComboBox.addItem(impulseResponseManager->getImpulseResponseName(i), (i + 1));
	}

	// set the presetComboBox's text to the currently selected preset
	impulseResponseComboBox.setText(impulseResponseManager->getCurrentImpulseResponseName());
}
