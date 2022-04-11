/*
  ==============================================================================

    BDBottomPanel.h
    Created: 5 May 2021 3:30:01pm
    Author:  Olorin

  ==============================================================================
*/

#include "../PanelBase.h"

#pragma once
class ProcessPanel : public PanelBase
{
public:
    ProcessPanel(BassDivisionAudioProcessor& inProcessor);
    ~ProcessPanel() = default;

    void paint(juce::Graphics& g) override;

    void resized() override;

private:

    struct EqPanel : public PanelBase
    {
        EqPanel(BassDivisionAudioProcessor& inProcessor);
        ~EqPanel() = default;

        void paint(juce::Graphics& g) override;
        void resized() override;

    private:

        juce::Slider sliders[7];
        juce::Label labels[7];
        UniquePtr<juce::AudioProcessorValueTreeState::SliderAttachment> attachments[7];
        juce::String names[7] = {"EQ_LOW_SHELF", "EQ_BAND_1", "EQ_BAND_2", "EQ_BAND_3",
                                 "EQ_BAND_4", "EQ_BAND_5", "EQ_HIGH_SHELF"};
        juce::String freqs[7] = {"Low Shelf", "200 Hz", "500 Hz", "1 KHz", "1.5 KHz",
                                 "2.5 KHz", "High Shelf"};

        DivisionVoidFonts fonts;

        DivisionVoid::CentreKnobLAF centreLAF;

        Runes runes[2];
    };

    struct CompressorPanel : public PanelBase
    {
        CompressorPanel(BassDivisionAudioProcessor& inProcessor);
        ~CompressorPanel() = default;

        void paint(juce::Graphics& g) override;
        void resized() override;

    private:

        juce::Slider sliders[8];
        juce::Label labels[8];
        UniquePtr<juce::AudioProcessorValueTreeState::SliderAttachment> attachments[8];
        juce::String names[8] = {"ATTACK", "RELEASE", "RATIO", "THRESHOLD",
                                 "KNEE", "HPF", "INPUT", "OUTPUT"};

        DivisionVoidFonts fonts;

        DivisionVoid::CentreKnobLAF centreLAF;
        DivisionVoid::NormalKnobLAF LAF;
    };

    EqPanel eqPanel;
    CompressorPanel compressorPanel;
};

class BandPanel : public PanelBase
{
public:
    BandPanel(BassDivisionAudioProcessor& inProcessor);
    ~BandPanel() = default;

    void paint(juce::Graphics& g) override;

    void resized() override;

private:

    struct BandCompPanel : public PanelBase
    {
        BandCompPanel(BassDivisionAudioProcessor& inProcessor, juce::String bandName);
        ~BandCompPanel() = default;

        void paint(juce::Graphics& g) override;
        void resized() override;

    private:

        juce::Slider sliders[6];
        juce::Label labels[6];
        UniquePtr<juce::AudioProcessorValueTreeState::SliderAttachment> attachments[6];
        juce::String names[6] = {"ATTACK", "RELEASE", "RATIO",
                                 "THRESHOLD", "INPUT", "OUTPUT"};

        DivisionVoid::Button soloButton;
        UniquePtr<juce::AudioProcessorValueTreeState::ButtonAttachment> soloButtonAttachment;

        juce::String band;

        DivisionVoidFonts fonts;
    };

    struct BandDistortionPanel : public PanelBase
    {
        BandDistortionPanel(BassDivisionAudioProcessor& inProcessor, juce::String bandName);
        ~BandDistortionPanel() = default;

        void paint(juce::Graphics& g) override;
        void resized() override;

    private:

        juce::Slider sliders[4];
        juce::Label labels[4];
        UniquePtr<juce::AudioProcessorValueTreeState::SliderAttachment> attachments[4];
        juce::String names[4] = {"GAIN", "MIX", "TYPE", "EDGE"};

        DivisionVoid::Button soloButton;
        UniquePtr<juce::AudioProcessorValueTreeState::ButtonAttachment> soloButtonAttachment;

        juce::String band;

        DivisionVoidFonts fonts;
    };

    struct BandCrossoverPanel : public PanelBase
    {
        BandCrossoverPanel(BassDivisionAudioProcessor& inProcessor, juce::String bandName);
        ~BandCrossoverPanel() = default;

        void paint(juce::Graphics& g) override;
        void resized() override;

    private:

        juce::Slider slider;
        juce::Label label;
        UniquePtr<juce::AudioProcessorValueTreeState::SliderAttachment> attachment;

        juce::String band;

        Runes runes[2];

        DivisionVoidFonts fonts;
    };

    BandCompPanel subPanel, lowPanel;
    BandDistortionPanel midPanel, highPanel;
    BandCrossoverPanel lowCrossoverPanel, midCrossoverPanel, highCrossoverPanel;
};

class BottomPanel : public PanelBase
{
public:
    BottomPanel(BassDivisionAudioProcessor& inProcessor);
	~BottomPanel();

	void paint(juce::Graphics& g) override;

	void resized() override;

private:
	ProcessPanel processPanel;
	BandPanel bandPanel;
	
	DivisionVoid::Button processButton;
	DivisionVoid::Button bandButton;

    DivisionVoidFonts fonts;
};


