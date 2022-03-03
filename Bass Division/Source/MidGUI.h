/*
  ==============================================================================

    MidGUI.h
    Created: 4 Jul 2020 4:03:38pm
    Author:  Olorin

  ==============================================================================
*/

#pragma once

#include "BDPanelBase.h"

#include "Runes.h"

#include "CentreKnobLAF.h"
#include "NormalKnobLAF.h"

//==============================================================================
/*
*/
class MidGUI  : public BDPanelBase, public juce::Button::Listener
{
public:
    MidGUI(BassDivisionAudioProcessor&);
    ~MidGUI() override;

    void paint (juce::Graphics&) override;
    void resized() override;

	void buttonClicked(juce::Button* button) override;

private:

	CentreKnobLAF centreLAF;
	NormalKnobLAF LAF;

	/****************************************************************************
	 * LABELS
	 ****************************************************************************/

	juce::Label gainLabel;
	juce::Label wetDryLabel;
	juce::Label typeLabel;
	juce::Label edgeLabel;

	/****************************************************************************
	 * SLIDERS
	 ****************************************************************************/
	
	juce::Slider gainSlider;
	juce::Slider wetDrySlider;
	juce::Slider typeSlider;
	juce::Slider edgeSlider;

	/****************************************************************************
	 * ATTACHMENTS
	 ****************************************************************************/

	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainSliderAttachment;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> wetDrySliderAttachment;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> typeSliderAttachment;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> edgeSliderAttachment;

	

	DivisionVoid::Button soloButton;

	std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> soloButtonAttachment;

	//Utilities utils;
	//juce::Font norse = utils.getNorseBoldFont();
	Runes runes[4];

	BassDivisionAudioProcessor& audioProcessor;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidGUI)
};
