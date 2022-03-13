/*
  ==============================================================================

    IrPanel.h
    Created: 12 Nov 2020 6:46:47pm
    Author:  Olorin

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../PluginProcessor.h"
#include "SynthLookAndFeel.h"

//==============================================================================
/*
*/
class ImpulseResponsePanel  : public juce::Component,
				 public juce::Timer
{
public:
	ImpulseResponsePanel(BassDivisionAudioProcessor&);
    ~ImpulseResponsePanel() override;

    void paint (juce::Graphics&) override;
    void resized() override;

	void timerCallback() override;

	void updateImpulseResponseComboBox();

private:

	DivisionVoid::Button engageImpulseResponse;
	DivisionVoid::Button loadImpulseResponse;

	juce::ComboBox impulseResponseComboBox;

	juce::Slider highPassIRKnob;
	juce::Label highPassLabel;

	std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> engageIRAttachment;

	juce::String currentImpulseResponseName{ "No IR Loaded" };
	juce::String presetDisplayName{ "" };

	SynthLookAndFeel lookAndFeel;

	BassDivisionAudioProcessor& audioProcessor;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ImpulseResponsePanel)
};
