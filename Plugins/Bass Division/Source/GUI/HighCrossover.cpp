/*
  ==============================================================================

    HighCrossover.cpp
    Created: 18 May 2021 3:28:58pm
    Author:  Olorin

  ==============================================================================
*/

#include "HighCrossover.h"

HighCrossover::HighCrossover(BassDivisionAudioProcessor& inProcessor) : BDPanelBase(inProcessor)
{
	highXoverSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
	highXoverSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 55, 12);
	highXoverSlider.setTextValueSuffix("Hz");
	highXoverSlider.setLookAndFeel(&centreLAF);
	addAndMakeVisible(&highXoverSlider);

	highXoverLabel.setText("High X-over", juce::dontSendNotification);
	highXoverLabel.attachToComponent(&highXoverSlider, true);
	highXoverLabel.setJustificationType(juce::Justification::centred);
	highXoverLabel.setColour(0x1000281, juce::Colours::black);
	//highXoverLabel.setFont(norse);
	addAndMakeVisible(highXoverLabel);

	highXoverSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(inProcessor.treeState, "HIGH_XOVER", highXoverSlider);

	for (auto i = 0; i < 2; ++i)
	{
		runes[i].setRune(i + 4);
		addAndMakeVisible(runes[i]);
	}
}

HighCrossover::~HighCrossover()
{
}

void HighCrossover::paint(juce::Graphics& g)
{
}

void HighCrossover::resized()
{
	auto height = getHeight();
	auto width = getWidth();

	auto knobSize = 40;

	highXoverSlider.setBounds(width / 2 - knobSize / 2, height / 2 - knobSize / 2, knobSize, knobSize);

	highXoverLabel.setFont(DVFonts::getBoldNorse(15));

	auto runeSize = 25;

	runes[0].setBounds(width / 2 - runeSize / 2, height / 4 * 1 - runeSize / 2, runeSize, runeSize);
	runes[1].setBounds(width / 2 - runeSize / 2, height / 4 * 3 - runeSize / 2, runeSize, runeSize);
}
