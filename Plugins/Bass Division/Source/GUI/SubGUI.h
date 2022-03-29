/*
  ==============================================================================

    SubGUI.h
    Created: 28 Feb 2021 5:38:55pm
    Author:  Olorin

  ==============================================================================
*/

#pragma once

//#include <JuceHeader.h>

#include "../../../BassDivision/Source/GUI/PanelBase.h"

#include "CentreKnobLAF.h"
#include "NormalKnobLAF.h"

//==============================================================================
/*
*/
class SubGUI  : public BDPanelBase, public juce::Button::Listener, juce::Timer
{
public:
    SubGUI(BassDivisionAudioProcessor&);
    ~SubGUI() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void buttonClicked(juce::Button* button) override;

private:

	void timerCallback() override;


	CentreKnobLAF centreLAF;
	NormalKnobLAF LAF;

	/****************************************************************************
	 * LABELS
	 ****************************************************************************/

	juce::Label attackLabel;
	juce::Label releaseLabel;
	juce::Label ratioLabel;
	juce::Label threshLabel;
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

	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> inputSliderAttachment;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> outputSliderAttachment;

	/****************************************************************************
	 * METERS
	 ****************************************************************************/

	// Metering has been put on the back burner for the moment. Something is very wrong there.

	 /****************************************************************************
	  * BUTTON STUFF
	  ****************************************************************************/

	DivisionVoid::Button soloButton;

	std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> soloButtonAttachment;
    
    void attachLabel(juce::Label label, juce::String string, juce::Component toAttachTo);

	

	//BassDivisionAudioProcessor& audioProcessor;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SubGUI)
};
