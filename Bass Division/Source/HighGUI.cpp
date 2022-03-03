/*
  ==============================================================================

    HighGUI.cpp
    Created: 4 Jul 2020 4:03:53pm
    Author:  Olorin

  ==============================================================================
*/

#include <JuceHeader.h>
#include "HighGUI.h"

//==============================================================================
HighGUI::HighGUI(BassDivisionAudioProcessor& p) : audioProcessor(p), BDPanelBase(p)
{
	setSize(350, 230);

	// Gain Slider
	gainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
	gainSlider.setRange(0, 12, 0.1f);
	gainSlider.setValue(0);
	gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 55, 12);
	gainSlider.setSkewFactorFromMidPoint(0);
	gainSlider.setTextValueSuffix("dB");
	gainSlider.setLookAndFeel(&centreLAF);
	addAndMakeVisible(&gainSlider);

	gainLabel.setText("Gain", juce::dontSendNotification);
	gainLabel.attachToComponent(&gainSlider, false);
	gainLabel.setJustificationType(juce::Justification::centred);
	gainLabel.setColour(0x1000281, juce::Colours::black);
	//gainLabel.setFont(norse);
	addAndMakeVisible(gainLabel);

	gainSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "HIGH_DISTORTION_GAIN", gainSlider);

	// Output Slider
	wetDrySlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
	wetDrySlider.setRange(-24, 6, 0.1f);
	wetDrySlider.setValue(0);
	wetDrySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 55, 12);
	wetDrySlider.setTextValueSuffix("%");
	wetDrySlider.setLookAndFeel(&LAF);
	addAndMakeVisible(&wetDrySlider);

	wetDryLabel.setText("Mix", juce::dontSendNotification);
	wetDryLabel.attachToComponent(&wetDrySlider, false);
	wetDryLabel.setJustificationType(juce::Justification::centred);
	wetDryLabel.setColour(0x1000281, juce::Colours::black);
	//wetDryLabel.setFont(norse);
	addAndMakeVisible(wetDryLabel);

	wetDrySliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "HIGH_DISTORTION_WETDRY", wetDrySlider);

	// Type Slider
	typeSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
	typeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 12);
	//ratioSlider.setTextValueSuffix("ms");
	typeSlider.setLookAndFeel(&centreLAF);
	addAndMakeVisible(&typeSlider);

	typeLabel.setText("Type", juce::dontSendNotification);
	typeLabel.attachToComponent(&typeSlider, false);
	typeLabel.setJustificationType(juce::Justification::centred);
	typeLabel.setColour(0x1000281, juce::Colours::black);
	//typeLabel.setFont(norse);
	addAndMakeVisible(typeLabel);

	typeSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "HIGH_DISTORTION_TYPE", typeSlider);

	// Edge Slider
	edgeSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
	edgeSlider.setRange(1, 5, 0.1f);
	edgeSlider.setValue(0);
	edgeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 55, 12);
	//edgeSlider.setTextValueSuffix("dB");
	edgeSlider.setLookAndFeel(&LAF);
	addAndMakeVisible(&edgeSlider);

	edgeLabel.setText("Edge", juce::dontSendNotification);
	edgeLabel.attachToComponent(&edgeSlider, false);
	edgeLabel.setJustificationType(juce::Justification::centred);
	edgeLabel.setColour(0x1000281, juce::Colours::black);
	//edgeLabel.setFont(norse);
	addAndMakeVisible(edgeLabel);

	edgeSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "HIGH_DISTORTION_EDGE", edgeSlider);

	//-----------------------------------------------------------------------------------------------------//

	soloButton.addListener(this);
	soloButton.setToggleState(false, juce::dontSendNotification);
	//soloButton.setBounds(10, 200, 25, 25);
	soloButton.setButtonStyle(DivisionVoid::Button::ButtonStyle::BarToggle);
	soloButton.setButtonText("S");
	//soloButton.setFont(norse);

	addAndMakeVisible(soloButton);

	soloButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, "HIGH_SOLO", soloButton);

	for (auto i = 0; i < 2; ++i)
	{
		runes[i].setRune(i + 2);
		//addAndMakeVisible(runes[i]);
	}
}

HighGUI::~HighGUI()
{
}

void HighGUI::paint(juce::Graphics& g)
{
	//BDPanelBase::paint(g);

	// Text to denote what section this is working on
	g.setColour(juce::Colours::black);
	g.setFont(DVFonts::getBoldNorse(25));
	//g.setFont(25);
	g.drawText("Highs", 0, 0, getWidth(), getHeight() - 15, juce::Justification::centredBottom);

}

void HighGUI::resized()
{
	auto bounds = getLocalBounds();

	const auto width = getWidth();
	const auto height = getHeight();

	const auto centreY = height / 2;

	const auto paramKnobSize = height / 5;

	const auto borderX = 10;
	const auto borderY = 20;
	const auto yOffset = 30;

	gainSlider.setBounds(borderX, borderY + yOffset, paramKnobSize, paramKnobSize);
	edgeSlider.setBounds(width - borderX - paramKnobSize, borderY + yOffset, paramKnobSize, paramKnobSize);
	wetDrySlider.setBounds(borderX, borderY + centreY, paramKnobSize, paramKnobSize);
	typeSlider.setBounds(width - borderX - paramKnobSize, borderY + centreY, paramKnobSize, paramKnobSize);

	

	const auto buttonWidth = 25;
	const auto buttonOffset = 10;

	soloButton.setBounds(buttonOffset, height - buttonOffset - buttonWidth, buttonWidth, buttonWidth);

	gainLabel.setFont(DVFonts::getBoldNorse(17));
	edgeLabel.setFont(DVFonts::getBoldNorse(17));
	wetDryLabel.setFont(DVFonts::getBoldNorse(17));
	typeLabel.setFont(DVFonts::getBoldNorse(17));

	soloButton.setFont(DVFonts::getBoldNorse(17));
	
}

void HighGUI::buttonClicked(juce::Button* button)
{
}
