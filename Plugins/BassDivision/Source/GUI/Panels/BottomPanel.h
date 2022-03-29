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

        DivisionVoidFonts fonts;
    };

    struct CompressorPanel : public PanelBase
    {
        CompressorPanel(BassDivisionAudioProcessor& inProcessor);
        ~CompressorPanel() = default;

        void paint(juce::Graphics& g) override;
        void resized() override;

    private:

        DivisionVoidFonts fonts;
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


};

class BottomPanel : public PanelBase
{
public:
    BottomPanel(BassDivisionAudioProcessor& inProcessor);
	~BottomPanel();

	void paint(juce::Graphics& g) override;

	void resized() override;

private:
	ProcessPanel mProcessPanel;
	BandPanel mBandPanel;
	
	DivisionVoid::Button processButton;
	DivisionVoid::Button bandButton;

    DivisionVoidFonts fonts;
};


