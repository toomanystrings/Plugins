/*
  ==============================================================================

    BDIOPanel.h
    Created: 20 Feb 2021 8:53:04pm
    Author:  Olorin

  ==============================================================================
*/

#pragma once

#include "BDPanelBase.h"

#include "PluginProcessor.h"
#include "SynthLookAndFeel.h"

#include "NormalKnobLAF.h"

class BDIOPanel : public BDPanelBase, private juce::Timer
{
public:
    BDIOPanel(BassDivisionAudioProcessor& inProcessor);
	~BDIOPanel();

	void paint(juce::Graphics& g) override;

	void resized() override;

	

private:

	void timerCallback() override;


	SynthLookAndFeel lookAndFeel;
	NormalKnobLAF LAF;

	/****************************************************************************
	 * LABELS
	 ****************************************************************************/

	juce::Label inputLabel;
	juce::Label outputLabel;

	/****************************************************************************
	 * SLIDERS
	 ****************************************************************************/

	juce::Slider inputSlider;
	juce::Slider outputSlider;

	/****************************************************************************
	 * ATTACHMENTS
	 ****************************************************************************/

	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> inputSliderAttachment;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> outputSliderAttachment;

	/****************************************************************************
	 * METERS
	 ****************************************************************************/


	//BassDivisionAudioProcessor& audioProcessor;
	
};
