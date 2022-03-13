/*
  ==============================================================================

    LowGUI.cpp
    Created: 4 Jul 2020 4:02:20pm
    Author:  Olorin

  ==============================================================================
*/

#include <JuceHeader.h>
#include "LowGUI.h"

//==============================================================================
LowGUI::LowGUI(BassDivisionAudioProcessor& audioProcessor) : BDPanelBase(audioProcessor)//, audioProcessor(*audioProcessor)
{
	//setSize(BAND_PANEL_WIDTH, BAND_PANEL_HEIGHT);

	startTimerHz(60);

	// Attack Slider
	attackSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
	attackSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 55, 12);
	attackSlider.setTextValueSuffix("ms");
	attackSlider.setLookAndFeel(&LAF);
	addAndMakeVisible(&attackSlider);

	attackLabel.setText("Attack", juce::dontSendNotification);
	attackLabel.attachToComponent(&attackSlider, false);
	attackLabel.setJustificationType(juce::Justification::centred);
	attackLabel.setColour(0x1000281, juce::Colours::black);
	addAndMakeVisible(attackLabel);

	attackSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "LOW_COMP_ATTACK", attackSlider);

	// Release Slider
	releaseSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
	releaseSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 55, 12);
	releaseSlider.setTextValueSuffix("ms");
	releaseSlider.setLookAndFeel(&LAF);
	addAndMakeVisible(&releaseSlider);

	releaseLabel.setText("Release", juce::dontSendNotification);
	releaseLabel.attachToComponent(&releaseSlider, false);
	releaseLabel.setJustificationType(juce::Justification::centred);
	releaseLabel.setColour(0x1000281, juce::Colours::black);
	addAndMakeVisible(releaseLabel);

	releaseSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "LOW_COMP_RELEASE", releaseSlider);

	// Ratio Slider
	ratioSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
	ratioSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 55, 12);
	ratioSlider.setLookAndFeel(&LAF);
	addAndMakeVisible(&ratioSlider);

	ratioLabel.setText("Ratio", juce::dontSendNotification);
	ratioLabel.attachToComponent(&ratioSlider, false);
	ratioLabel.setJustificationType(juce::Justification::centred);
	ratioLabel.setColour(0x1000281, juce::Colours::black);
	addAndMakeVisible(ratioLabel);

	ratioSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "LOW_COMP_RATIO", ratioSlider);

	// Threshold Slider
	threshSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
	threshSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 55, 12);
	threshSlider.setTextValueSuffix("dB");
	threshSlider.setLookAndFeel(&LAF);
	addAndMakeVisible(&threshSlider);

	threshLabel.setText("Threshold", juce::dontSendNotification);
	threshLabel.attachToComponent(&threshSlider, false);
	threshLabel.setJustificationType(juce::Justification::centred);
	threshLabel.setColour(0x1000281, juce::Colours::black);
	addAndMakeVisible(threshLabel);

	threshSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "LOW_COMP_THRESHOLD", threshSlider);

	// Input Slider
	inputSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
	inputSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 55, 12);
	inputSlider.setSkewFactorFromMidPoint(0.0);
	inputSlider.setTextValueSuffix("dB");
	inputSlider.setLookAndFeel(&centreLAF);
	addAndMakeVisible(&inputSlider);

	inputLabel.setText("Input", juce::dontSendNotification);
	inputLabel.attachToComponent(&inputSlider, false);
	inputLabel.setJustificationType(juce::Justification::centred);
	inputLabel.setColour(0x1000281, juce::Colours::black);
	addAndMakeVisible(inputLabel);

	inputSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "LOW_COMP_INPUT", inputSlider);

	// Output Slider
	outputSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
	outputSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 55, 12);
	outputSlider.setSkewFactorFromMidPoint(0.0);
	outputSlider.setTextValueSuffix("dB");
	outputSlider.setLookAndFeel(&centreLAF);
	addAndMakeVisible(&outputSlider);

	outputLabel.setText("Output", juce::dontSendNotification);
	outputLabel.attachToComponent(&outputSlider, false);
	outputLabel.setJustificationType(juce::Justification::centred);
	outputLabel.setColour(0x1000281, juce::Colours::black);
	addAndMakeVisible(outputLabel);

	outputSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "LOW_COMP_OUTPUT", outputSlider);

	//-----------------------------------------------------------------------------------------------------//

	// Meter stuff went here
    
	//-----------------------------------------------------------------------------------------------------//

	soloButton.addListener(this);
	soloButton.setToggleState(false, juce::dontSendNotification);
	soloButton.setButtonStyle(DivisionVoid::Button::ButtonStyle::BarToggle);
	soloButton.setButtonText("S");

	addAndMakeVisible(soloButton);

	soloButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, "LOW_SOLO", soloButton);
}

LowGUI::~LowGUI()
{
}

void LowGUI::paint (juce::Graphics& g)
{
	//BDPanelBase::paint(g);

	g.setFont(DVFonts::getBoldNorse(25));
	//g.setFont(25);
	g.drawText("LOWS", 0, 0, getWidth(), getHeight() - 15, juce::Justification::centredBottom);
}

void LowGUI::resized()
{
	//auto area = getBounds();

	const auto width = getWidth();
	const auto height = getHeight();

	const auto centreX = width / 2 + 20;
	//const auto centreY = height / 2;

	const auto gainKnobSize = height / 5.5;


	//const auto border = 0;

	auto row1 = 35;
	auto row2 = 125;
	auto row3 = 215;

	attackSlider.setBounds(centreX - gainKnobSize, row1, gainKnobSize, gainKnobSize);
	releaseSlider.setBounds(centreX, row1, gainKnobSize, gainKnobSize);

	threshSlider.setBounds(centreX - gainKnobSize, row2, gainKnobSize, gainKnobSize);
	ratioSlider.setBounds(centreX, row2, gainKnobSize, gainKnobSize);

	inputSlider.setBounds(centreX - gainKnobSize, row3, gainKnobSize, gainKnobSize);
	outputSlider.setBounds(centreX, row3, gainKnobSize, gainKnobSize);


	const auto buttonWidth = 25;
	const auto buttonOffset = 10;

	soloButton.setBounds(buttonOffset + buttonWidth + 15, height - buttonOffset - buttonWidth, buttonWidth, buttonWidth);

	attackLabel.setFont(DVFonts::getBoldNorse(17));
	releaseLabel.setFont(DVFonts::getBoldNorse(17));
	threshLabel.setFont(DVFonts::getBoldNorse(17));
	ratioLabel.setFont(DVFonts::getBoldNorse(17));
	inputLabel.setFont(DVFonts::getBoldNorse(17));
	outputLabel.setFont(DVFonts::getBoldNorse(17));

	soloButton.setFont(DVFonts::getBoldNorse(17));
}

void LowGUI::buttonClicked(juce::Button* button)
{
}

void LowGUI::timerCallback()
{
	
}
