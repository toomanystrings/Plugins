/*
  ==============================================================================

    BDIOPanel.cpp
    Created: 20 Feb 2021 8:53:04pm
    Author:  Olorin

  ==============================================================================
*/

#include "BDIOPanel.h"

BDIOPanel::BDIOPanel(BassDivisionAudioProcessor& inProcessor) : BDPanelBase(inProcessor)//, audioProcessor(inProcessor)
{
    setSize(IO_PANEL_WIDTH, IO_PANEL_HEIGHT);

	// Input Slider
	inputSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);

	inputSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 55, 12);
	inputSlider.setTextValueSuffix("dB");
	inputSlider.setLookAndFeel(&LAF);
	addAndMakeVisible(&inputSlider);
	inputSlider.setSkewFactorFromMidPoint(0);

	inputLabel.setText("Input", juce::dontSendNotification);
	inputLabel.attachToComponent(&inputSlider, false);
	inputLabel.setJustificationType(juce::Justification::centred);
	inputLabel.setColour(0x1000281, juce::Colours::black);
	addAndMakeVisible(inputLabel);

	inputSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(inProcessor.treeState, "INPUT", inputSlider);

	// Output Slider
	outputSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);

	outputSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 55, 12);
	outputSlider.setTextValueSuffix("dB");
	outputSlider.setLookAndFeel(&LAF);
	addAndMakeVisible(&outputSlider);
	outputSlider.setSkewFactorFromMidPoint(0);

	outputLabel.setText("Output", juce::dontSendNotification);
	outputLabel.attachToComponent(&outputSlider, false);
	outputLabel.setJustificationType(juce::Justification::centred);
	outputLabel.setColour(0x1000281, juce::Colours::black);
	addAndMakeVisible(outputLabel);

	outputSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(inProcessor.treeState, "OUTPUT", outputSlider);



	//-----------------------------------------------------------------------------------------------------//

	
	
	//-----------------------------------------------------------------------------------------------------//
}

BDIOPanel::~BDIOPanel()
{
}

void BDIOPanel::paint(juce::Graphics& g)
{
    //BDPanelBase::paint(g);
}

void BDIOPanel::resized()
{
	auto bounds = getBounds();

	auto border = 25;

	const auto width = getWidth();
	const auto height = getHeight();

	const auto centreX = width / 2;
	const auto centreY = height / 2;

	const auto gainSliderSize = 40;
	

	const auto gainY = 25;


	inputSlider.setBounds(centreX - gainSliderSize - 5, 30, gainSliderSize, 260);
	outputSlider.setBounds(centreX + 5, 30, gainSliderSize, 260);


	inputLabel.setFont(DVFonts::getBoldNorse(17));
	outputLabel.setFont(DVFonts::getBoldNorse(17));
}

void BDIOPanel::timerCallback()
{
}
