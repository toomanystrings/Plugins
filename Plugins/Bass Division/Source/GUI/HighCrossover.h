/*
  ==============================================================================

    HighCrossover.h
    Created: 18 May 2021 3:28:58pm
    Author:  Olorin

  ==============================================================================
*/

#pragma once

#include "../../../BassDivision/Source/GUI/PanelBase.h"
#include "CentreKnobLAF.h"
#include "Runes.h"

class HighCrossover :public BDPanelBase
{
public:
    HighCrossover(BassDivisionAudioProcessor& inProcessor);
	~HighCrossover();

	void paint(juce::Graphics& g) override;

	void resized() override;

private:

	CentreKnobLAF centreLAF;
	
	juce::Slider highXoverSlider;
	juce::Label highXoverLabel;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> highXoverSliderAttachment;

	Runes runes[2];
};
