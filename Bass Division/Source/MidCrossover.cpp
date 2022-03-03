/*
  ==============================================================================

    MidCrossover.cpp
    Created: 18 May 2021 3:28:49pm
    Author:  Olorin

  ==============================================================================
*/

#include "MidCrossover.h"

MidCrossover::MidCrossover(BassDivisionAudioProcessor& inProcessor) : BDPanelBase(inProcessor)
{
	midXoverSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
	midXoverSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 55, 12);
	midXoverSlider.setTextValueSuffix("Hz");
	midXoverSlider.setLookAndFeel(&centreLAF);
	addAndMakeVisible(&midXoverSlider);

	midXoverLabel.setText("Mid X-over", juce::dontSendNotification);
	midXoverLabel.attachToComponent(&midXoverSlider, true);
	midXoverLabel.setJustificationType(juce::Justification::centred);
	midXoverLabel.setColour(0x1000281, juce::Colours::black);
	//midXoverLabel.setFont(norse);
	addAndMakeVisible(midXoverLabel);

	midXoverSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(inProcessor.treeState, "MID_XOVER", midXoverSlider);

	for (auto i = 0; i < 2; ++i)
	{
		runes[i].setRune(i + 2);
		addAndMakeVisible(runes[i]);
	}
}

MidCrossover::~MidCrossover()
{
}

void MidCrossover::paint(juce::Graphics& g)
{
}

void MidCrossover::resized()
{
	auto height = getHeight();
	auto width = getWidth();

	auto knobSize = 40;

	midXoverSlider.setBounds(width / 2 - knobSize / 2, height / 2 - knobSize / 2, knobSize, knobSize);

	midXoverLabel.setFont(DVFonts::getBoldNorse(15));

	auto runeSize = 25;

	runes[0].setBounds(width / 2 - runeSize / 2, height / 4 * 1 - runeSize / 2, runeSize, runeSize);
	runes[1].setBounds(width / 2 - runeSize / 2, height / 4 * 3 - runeSize / 2, runeSize, runeSize);
}
