/*
  ==============================================================================

    LowCrossover.h
    Created: 18 May 2021 3:28:39pm
    Author:  Olorin

  ==============================================================================
*/

#pragma once

#include "BDPanelBase.h"
#include "CentreKnobLAF.h"
#include "Runes.h"

class LowCrossover : public BDPanelBase
{
public:
    LowCrossover(BassDivisionAudioProcessor& inProcessor);
	~LowCrossover();

	void paint(juce::Graphics& g) override;

	void resized() override;

private:

	CentreKnobLAF centreLAF;

	juce::Slider lowXoverSlider;
	juce::Label lowXoverLabel;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lowXoverSliderAttachment;

	Runes runes[2];
};