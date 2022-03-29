/*
  ==============================================================================

    BDEqPanel.h
    Created: 21 Feb 2021 3:58:24pm
    Author:  Olorin

  ==============================================================================
*/

#pragma once

#include "../../../BassDivision/Source/GUI/PanelBase.h"
#include "Runes.h"
#include "CentreKnobLAF.h"

class BDEqPanel : public BDPanelBase
{
public:
	BDEqPanel(BassDivisionAudioProcessor& inProcessor);
	~BDEqPanel();

	void paint(juce::Graphics& g) override;
	void resized() override;

private:

	CentreKnobLAF centreLAF;

	/****************************************************************************
	 * LABELS
	 ****************************************************************************/

	juce::Label lowShelfLabel;
	juce::Label band1Label;
	juce::Label band2Label;
	juce::Label band3Label;
	juce::Label band4Label;
	juce::Label band5Label;
	juce::Label highShelfLabel;

	/****************************************************************************
	 * SLIDERS
	 ****************************************************************************/


	juce::Slider lowShelfSlider;
	juce::Slider band1Slider;
	juce::Slider band2Slider;
	juce::Slider band3Slider;
	juce::Slider band4Slider;
	juce::Slider band5Slider;
	juce::Slider highShelfSlider;

	/****************************************************************************
	 * ATTACHMENTS
	 ****************************************************************************/

	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lowShelfSliderAttachment;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> band1SliderAttachment;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> band2SliderAttachment;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> band3SliderAttachment;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> band4SliderAttachment;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> band5SliderAttachment;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> highShelfSliderAttachment;



	Runes runes[2];

};
