/*
  ==============================================================================

    MidCrossover.h
    Created: 18 May 2021 3:28:49pm
    Author:  Olorin

  ==============================================================================
*/

#pragma once

#include "../../../BassDivision/Source/GUI/PanelBase.h"
#include "CentreKnobLAF.h"
#include "DivisionVoid/Source/DivisionVoidLookAndFeel/Runes.h"

class MidCrossover : public BDPanelBase
{
public:
    MidCrossover(BassDivisionAudioProcessor& inProcessor);
	~MidCrossover();

	void paint(juce::Graphics& g) override;

	void resized() override;

private:

	CentreKnobLAF centreLAF;
	
	juce::Slider midXoverSlider;
	juce::Label midXoverLabel;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> midXoverSliderAttachment;

	Runes runes[2];
};
