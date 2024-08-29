//
// Created by Conor on 11/04/2022.
//

#pragma once

#include "../PanelBase.h"
//#include "../../FFT/SpectrumViewer.h"

struct IoPanel : public PanelBase
{
    IoPanel(BassDivisionAudioProcessor& inProcessor);
    ~IoPanel() = default;

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

/*struct FftPanel : public PanelBase
{
    FftPanel (BassDivisionAudioProcessor& inProcessor,
            juce::Value& repaintSpectrumViewer,
            drow::Buffer& spectrumMagnitudeBuffer,
            juce::Value& detectedFrequency);
    ~FftPanel() = default;

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    // Values needed for the spectrum viewer, plus the component itself
    SpectrumViewer spectrumViewer;
    juce::Value sampleRate;
    juce::Label header;

    //BassDivisionAudioProcessor& audioProcessor;
};*/

class MiddlePanel : public PanelBase
{
public:
    MiddlePanel(BassDivisionAudioProcessor& inProcessor);
    ~MiddlePanel() = default;

    void paint(juce::Graphics& g) override;

    void resized() override;

private:
    IoPanel ioPanel;
    //FftPanel fftPanel;
};