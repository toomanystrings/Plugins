/*
  ==============================================================================

    BDCompPanel.cpp
    Created: 1 Mar 2021 1:15:01pm
    Author:  Olorin

  ==============================================================================
*/

#include "BDCompPanel.h"

BDCompPanel::BDCompPanel(BassDivisionAudioProcessor& inProcessor) : BDPanelBase(inProcessor)//, audioProcessor(*inProcessor)
{
	startTimerHz(30);
	

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

	attackSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(inProcessor.treeState, "COMP_ATTACK", attackSlider);

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

	releaseSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(inProcessor.treeState, "COMP_RELEASE", releaseSlider);

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

	ratioSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(inProcessor.treeState, "COMP_RATIO", ratioSlider);

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

	threshSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(inProcessor.treeState, "COMP_THRESHOLD", threshSlider);

	// Knee Slider
	kneeSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
	kneeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 55, 12);
	kneeSlider.setLookAndFeel(&LAF);
	addAndMakeVisible(&kneeSlider);

	kneeLabel.setText("Knee", juce::dontSendNotification);
	kneeLabel.attachToComponent(&kneeSlider, false);
	kneeLabel.setJustificationType(juce::Justification::centred);
	kneeLabel.setColour(0x1000281, juce::Colours::black);
	addAndMakeVisible(kneeLabel);

	kneeSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(inProcessor.treeState, "COMP_KNEE", kneeSlider);

	// HPF Slider
	hpfSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
	hpfSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 55, 12);
	hpfSlider.setTextValueSuffix("Hz");
	hpfSlider.setLookAndFeel(&LAF);
	addAndMakeVisible(&hpfSlider);

	hpfLabel.setText("HPF", juce::dontSendNotification);
	hpfLabel.attachToComponent(&hpfSlider, false);
	hpfLabel.setJustificationType(juce::Justification::centred);
	hpfLabel.setColour(0x1000281, juce::Colours::black);
	addAndMakeVisible(hpfLabel);

	hpfSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(inProcessor.treeState, "COMP_HPF", hpfSlider);

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

	inputSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(inProcessor.treeState, "COMP_INPUT", inputSlider);

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

	outputSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(inProcessor.treeState, "COMP_OUTPUT", outputSlider);

    
}

BDCompPanel::~BDCompPanel()
{
}

void BDCompPanel::paint(juce::Graphics& g)
{
	//BDPanelBase::paint(g);

	g.setFont(DVFonts::getBoldNorse(25));
	//g.setFont(25);
	g.drawText("Compressor", 0, 0, getWidth() - 15, getHeight() - 15, juce::Justification::centredBottom);
}

void BDCompPanel::resized()
{
	// Basic calculations for the layout. Realistically this should be dynamic, in a grid layout or using methods provided by juce, but this
	//should work for now
	const auto width = getWidth();
	const auto height = getHeight();

	const auto centreY = height / 2;

	const auto knobDiameter = height / 4;

	const auto border = 20;
	const auto yOffset = 30;

	// Threshold Ratio Attack Release
	// Input Knee HPF Output
	
	threshSlider.setBounds((width / 6 * 1), border + yOffset, knobDiameter, knobDiameter);
	ratioSlider.setBounds((width / 6 * 2), border + yOffset, knobDiameter, knobDiameter);
	attackSlider.setBounds((width / 6 * 3), border + yOffset, knobDiameter, knobDiameter);
	releaseSlider.setBounds((width / 6 * 4), border + yOffset, knobDiameter, knobDiameter);

	inputSlider.setBounds((width / 6 * 1), border + centreY, knobDiameter, knobDiameter);
	kneeSlider.setBounds((width / 6 * 2), border + centreY, knobDiameter, knobDiameter);
	hpfSlider.setBounds((width / 6 * 3), border + centreY, knobDiameter, knobDiameter);
	outputSlider.setBounds((width / 6 * 4), border + centreY, knobDiameter, knobDiameter);

	threshLabel.setFont(DVFonts::getBoldNorse(17));
	ratioLabel.setFont(DVFonts::getBoldNorse(17));
	attackLabel.setFont(DVFonts::getBoldNorse(17));
	releaseLabel.setFont(DVFonts::getBoldNorse(17));
	inputLabel.setFont(DVFonts::getBoldNorse(17));
	kneeLabel.setFont(DVFonts::getBoldNorse(17));
	hpfLabel.setFont(DVFonts::getBoldNorse(17));
	outputLabel.setFont(DVFonts::getBoldNorse(17));
}

void BDCompPanel::timerCallback()
{
}
