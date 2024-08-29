//
// Created by Conor Foran on 12/04/2022.
//

#include "TopPanel.h"

IrPanel::IrPanel(BassDivisionAudioProcessor& inProcessor) : PanelBase(inProcessor), audioProcessor(inProcessor)
{
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

IrPanel::~IrPanel()
{
    stopTimer();
}

void IrPanel::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::whitesmoke.withAlpha(0.f));
}

void IrPanel::resized()
{
    auto area = getLocalBounds();
    auto border = 4;

    auto buttonArea = area.removeFromLeft(area.getWidth() * .5);

    loadImpulseResponse.setBounds(buttonArea.removeFromLeft(buttonArea.getWidth() / 2).reduced(border));
    engageImpulseResponse.setBounds(buttonArea.removeFromLeft(buttonArea.getWidth()).reduced(border));
    impulseResponseComboBox.setBounds(area.reduced(border));

    loadImpulseResponse.setFont(fonts.getFont(DivisionVoidFonts::FontType::bold, 15));
    engageImpulseResponse.setFont(fonts.getFont(DivisionVoidFonts::FontType::bold, 15));
}

void IrPanel::timerCallback()
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

void IrPanel::updateImpulseResponseComboBox()
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

PresetPanel::PresetPanel(BassDivisionAudioProcessor& inProcessor): PanelBase(inProcessor), audioProcessor(inProcessor)
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

    savePreset.setFont(fonts.getFont(DivisionVoidFonts::FontType::bold, 15));
}

TopPanel::TopPanel(BassDivisionAudioProcessor &inProcessor) : PanelBase(inProcessor),
                                                              presetPanel(inProcessor),
                                                              irPanel(inProcessor)
{
    addAndMakeVisible(presetPanel);
    addAndMakeVisible(irPanel);
}

void TopPanel::paint(juce::Graphics &g)
{}

void TopPanel::resized()
{
    auto area = getLocalBounds();
    auto halfWidth = getWidth() / 2;

    irPanel.setBounds(halfWidth, 0, halfWidth - 1, getHeight());
    presetPanel.setBounds(0, 0, halfWidth, getHeight());
}

