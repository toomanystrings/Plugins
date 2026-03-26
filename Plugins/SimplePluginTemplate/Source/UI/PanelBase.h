//
// Created by Conor Foran on 20/03/2026.
//

#pragma once

#include "../PluginProcessor.h"

class Panel : public DivisionVoid::PanelBase
{
public:
    Panel(NewPluginTemplateAudioProcessor& processor) : DivisionVoid::PanelBase(processor)
    {
        setSize(getParentWidth(), getParentHeight());
        slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
        slider.hideTextBox(true);

        MakeUnique<juce::AudioProcessorValueTreeState::SliderAttachment>()

        addAndMakeVisible(&slider);


    }
    ~Panel() {}

private:
    void resized() override
    {
        slider.setBounds(getBounds());
    }

    DivisionVoid::DoubleStackKnob doubleStackKnob;
    juce::Slider slider;

    UniquePtr<juce::AudioProcessorValueTreeState::SliderAttachment> attachments[2];
    juce::String names[2] {"Inner", "Outer"};
};
