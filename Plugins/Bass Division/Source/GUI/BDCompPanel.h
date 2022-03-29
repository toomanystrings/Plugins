/*
  ==============================================================================

    BDCompPanel.h
    Created: 1 Mar 2021 1:15:01pm
    Author:  Olorin

  ==============================================================================
*/

#pragma once

#include "../../../BassDivision/Source/GUI/PanelBase.h"
#include "CentreKnobLAF.h"
#include "NormalKnobLAF.h"

class BDCompPanel : public BDPanelBase, juce::Timer
{
public:
    BDCompPanel(BassDivisionAudioProcessor& inProcessor);
	~BDCompPanel();

	void paint(juce::Graphics& g) override;

	void resized() override;

	void timerCallback() override;

private:

	CentreKnobLAF centreLAF;
	NormalKnobLAF LAF;

	/****************************************************************************
	 * LABELS
	 ****************************************************************************/

	juce::Label attackLabel;
	juce::Label releaseLabel;
	juce::Label ratioLabel;
	juce::Label threshLabel;
	juce::Label kneeLabel;
	juce::Label hpfLabel;

	juce::Label inputLabel;
	juce::Label outputLabel;

	/****************************************************************************
	 * SLIDERS
	 ****************************************************************************/

	 // Compression parameter sliders
	juce::Slider attackSlider;
	juce::Slider releaseSlider;
	juce::Slider ratioSlider;
	juce::Slider threshSlider;
	juce::Slider kneeSlider;
	juce::Slider hpfSlider;

	// In/Out sliders
	juce::Slider inputSlider;
	juce::Slider outputSlider;

	/****************************************************************************
	 * ATTACHMENTS
	 ****************************************************************************/

	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attackSliderAttachment;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> releaseSliderAttachment;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> ratioSliderAttachment;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> threshSliderAttachment;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> kneeSliderAttachment;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> hpfSliderAttachment;

	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> inputSliderAttachment;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> outputSliderAttachment;

	/****************************************************************************
	 * METERS AND MISC
	 ****************************************************************************/

    

	//BassDivisionAudioProcessor& audioProcessor;
};
