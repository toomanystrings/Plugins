/*
  ==============================================================================

    LowCrossover.cpp
    Created: 18 May 2021 3:28:39pm
    Author:  Olorin

  ==============================================================================
*/

#include "LowCrossover.h"

LowCrossover::LowCrossover(BassDivisionAudioProcessor& inProcessor) : BDPanelBase(inProcessor)
{
	lowXoverSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
	lowXoverSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 55, 12);
	lowXoverSlider.setTextValueSuffix("Hz");
	lowXoverSlider.setLookAndFeel(&centreLAF);
	addAndMakeVisible(&lowXoverSlider);

	lowXoverLabel.setText("Low X-over", juce::dontSendNotification);
	lowXoverLabel.attachToComponent(&lowXoverSlider, true);
	lowXoverLabel.setJustificationType(juce::Justification::centred);
	lowXoverLabel.setColour(0x1000281, juce::Colours::black);
	//lowXoverLabel.setFont(norse);
	addAndMakeVisible(lowXoverLabel);

	lowXoverSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(inProcessor.treeState, "LOW_XOVER", lowXoverSlider);

	for (auto i = 0; i < 2; ++i)
	{
		runes[i].setRune(i);
		addAndMakeVisible(runes[i]);
	}
}

LowCrossover::~LowCrossover()
{
}

void LowCrossover::paint(juce::Graphics& g)
{
}

void LowCrossover::resized()
{
	auto height = getHeight();
	auto width = getWidth();

	auto knobSize = 40;

	

	lowXoverSlider.setBounds(width / 2 - knobSize / 2, height / 2 - knobSize / 2, knobSize, knobSize);

	lowXoverLabel.setFont(DVFonts::getBoldNorse(15));

	auto runeSize = 25;

	runes[0].setBounds(width / 2 - runeSize / 2, height / 4 * 1 - runeSize / 2, runeSize, runeSize);
	runes[1].setBounds(width / 2 - runeSize / 2, height / 4 * 3 - runeSize / 2, runeSize, runeSize);
	
}
