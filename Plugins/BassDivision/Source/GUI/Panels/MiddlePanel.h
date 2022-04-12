//
// Created by Conor on 11/04/2022.
//

#pragma once

#include "../PanelBase.h"

struct IoPanel : public PanelBase
{
    IoPanel(BassDivisionAudioProcessor& inProcessor);
    ~IoPanel();

    void paint(juce::Graphics& g) override;

    void resized() override;

private:

    // For meters, when they are sorted out
    //void timerCallback() override;

    DivisionVoid::NormalKnobLAF LAF;

    juce::Slider sliders[2];
    juce::Label labels[2];
    UniquePtr<juce::AudioProcessorValueTreeState::SliderAttachment> attachments[2];
    juce::String names[2] = {"INPUT", "OUTPUT"};

    DivisionVoidFonts fonts;
};

struct FftPanel : public PanelBase
{

};

class MiddlePanel : public PanelBase
{
    MiddlePanel(BassDivisionAudioProcessor& inProcessor);
    ~MiddlePanel() = default;

    void paint(juce::Graphics& g) override;

    void resized() override;

private:


};