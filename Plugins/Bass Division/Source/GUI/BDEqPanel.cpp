/*
  ==============================================================================

    BDEqPanel.cpp
    Created: 21 Feb 2021 3:58:24pm
    Author:  Olorin

  ==============================================================================
*/

#include "BDEqPanel.h"

BDEqPanel::BDEqPanel(BassDivisionAudioProcessor& inProcessor) : BDPanelBase(inProcessor)
{
    //setSize(EQ_PANEL_WIDTH, EQ_PANEL_HEIGHT);

	lowShelfSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
	lowShelfSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 55, 12);
	lowShelfSlider.setTextValueSuffix("dB");
	lowShelfSlider.setLookAndFeel(&centreLAF);
	addAndMakeVisible(&lowShelfSlider);

	lowShelfLabel.setText("Low Shelf", juce::dontSendNotification);
	lowShelfLabel.attachToComponent(&lowShelfSlider, false);
	lowShelfLabel.setJustificationType(juce::Justification::centred);
	lowShelfLabel.setColour(0x1000281, juce::Colours::black);
	//lowShelfLabel.setFont(norse);
	addAndMakeVisible(lowShelfLabel);

	lowShelfSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(inProcessor.treeState, "EQ_LOW_SHELF", lowShelfSlider);

	band1Slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
	band1Slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 55, 12);
	band1Slider.setTextValueSuffix("dB");
	band1Slider.setLookAndFeel(&centreLAF);
	addAndMakeVisible(&band1Slider);

	band1Label.setText("200 Hz", juce::dontSendNotification);
	band1Label.attachToComponent(&band1Slider, false);
	band1Label.setJustificationType(juce::Justification::centred);
	band1Label.setColour(0x1000281, juce::Colours::black);
	//band1Label.setFont(norse);
	addAndMakeVisible(band1Label);

	band1SliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(inProcessor.treeState, "EQ_BAND_1", band1Slider);

	band2Slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
	band2Slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 55, 12);
	band2Slider.setTextValueSuffix("dB");
	band2Slider.setLookAndFeel(&centreLAF);

	addAndMakeVisible(&band2Slider);

	band2Label.setText("500 Hz", juce::dontSendNotification);
	band2Label.attachToComponent(&band2Slider, false);
	band2Label.setJustificationType(juce::Justification::centred);
	band2Label.setColour(0x1000281, juce::Colours::black);
	//band2Label.setFont(norse);
	addAndMakeVisible(band2Label);

	band2SliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(inProcessor.treeState, "EQ_BAND_2", band2Slider);

	band3Slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
	band3Slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 55, 12);
	band3Slider.setTextValueSuffix("dB");
	band3Slider.setLookAndFeel(&centreLAF);
	addAndMakeVisible(&band3Slider);

	band3Label.setText("1 kHz", juce::dontSendNotification);
	band3Label.attachToComponent(&band3Slider, false);
	band3Label.setJustificationType(juce::Justification::centred);
	band3Label.setColour(0x1000281, juce::Colours::black);
	//band3Label.setFont(norse);
	addAndMakeVisible(band3Label);

	band3SliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(inProcessor.treeState, "EQ_BAND_3", band3Slider);

	band4Slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
	band4Slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 55, 12);
	band4Slider.setTextValueSuffix("dB");
	band4Slider.setLookAndFeel(&centreLAF);
	addAndMakeVisible(&band4Slider);

	band4Label.setText("1.5 kHz", juce::dontSendNotification);
	band4Label.attachToComponent(&band4Slider, false);
	band4Label.setJustificationType(juce::Justification::centred);
	band4Label.setColour(0x1000281, juce::Colours::black);
	//band4Label.setFont(norse);
	addAndMakeVisible(band4Label);

	band4SliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(inProcessor.treeState, "EQ_BAND_4", band4Slider);

	band5Slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
	band5Slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 55, 12);
	band5Slider.setTextValueSuffix("dB");
	band5Slider.setLookAndFeel(&centreLAF);
	addAndMakeVisible(&band5Slider);

	band5Label.setText("2.5 kHz", juce::dontSendNotification);
	band5Label.attachToComponent(&band5Slider, false);
	band5Label.setJustificationType(juce::Justification::centred);
	band5Label.setColour(0x1000281, juce::Colours::black);
	//band5Label.setFont(norse);
	addAndMakeVisible(band5Label);

	band5SliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(inProcessor.treeState, "EQ_BAND_5", band5Slider);

	highShelfSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
	highShelfSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 55, 12);
	lowShelfSlider.setTextValueSuffix("dB");
	highShelfSlider.setLookAndFeel(&centreLAF);
	addAndMakeVisible(&highShelfSlider);

	highShelfLabel.setText("High Shelf", juce::dontSendNotification);
	highShelfLabel.attachToComponent(&highShelfSlider, false);
	highShelfLabel.setJustificationType(juce::Justification::centred);
	highShelfLabel.setColour(0x1000281, juce::Colours::black);
	//highShelfLabel.setFont(norse);
	addAndMakeVisible(highShelfLabel);

	highShelfSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(inProcessor.treeState, "EQ_HIGH_SHELF", highShelfSlider);

	

	for (auto i = 0; i < 2; ++i)
	{
		runes[i].setRune(i);
		addAndMakeVisible(runes[i]);
	}
}

BDEqPanel::~BDEqPanel()
{
}

void BDEqPanel::paint(juce::Graphics& g)
{
    //BDPanelBase::paint(g);

	g.setFont(DVFonts::getBoldNorse(25));
	//g.setFont(25);
	g.drawText("EQUALISER", 0, 0, getWidth() - 15, getHeight() - 15, juce::Justification::centredBottom);
}

void BDEqPanel::resized()
{
	const auto width = getWidth();
	const auto height = getHeight();

	const auto centreY = height / 2;

	const auto knobDiameter = height / 4;

	const auto border = 20;
	const auto yOffset = 30;

	lowShelfSlider.setBounds((width / 6 * 1), border + yOffset, knobDiameter, knobDiameter);
	band1Slider.setBounds((width / 6 * 1.5), centreY + border, knobDiameter, knobDiameter);
	band2Slider.setBounds((width / 6 * 2), border + yOffset, knobDiameter, knobDiameter);
	band3Slider.setBounds((width / 6 * 2.5), centreY + border, knobDiameter, knobDiameter);
	band4Slider.setBounds((width / 6 * 3), border + yOffset, knobDiameter, knobDiameter);
	band5Slider.setBounds((width / 6 * 3.5), centreY + border, knobDiameter, knobDiameter);
	highShelfSlider.setBounds((width / 6 * 4), border + yOffset, knobDiameter, knobDiameter);


	runes[0].setBounds(40, centreY - 10, 20, 20);
	runes[1].setBounds(width - 60, centreY - 10, 20, 20);

	lowShelfLabel.setFont(DVFonts::getBoldNorse(17));
	band1Label.setFont(DVFonts::getBoldNorse(17));
	band2Label.setFont(DVFonts::getBoldNorse(17));
	band3Label.setFont(DVFonts::getBoldNorse(17));
	band4Label.setFont(DVFonts::getBoldNorse(17));
	band5Label.setFont(DVFonts::getBoldNorse(17));
	highShelfLabel.setFont(DVFonts::getBoldNorse(17));
}
