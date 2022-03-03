/*
  ==============================================================================

    BDTopPanel.h
    Created: 20 Feb 2021 8:53:19pm
    Author:  Olorin

  ==============================================================================
*/

#pragma once

#include "BDPanelBase.h"

#include "PresetPanel.h"
#include "IrPanel.h"

class BDTopPanel : public BDPanelBase
{
public:
    BDTopPanel(BassDivisionAudioProcessor& inProcessor);
    ~BDTopPanel();

    void paint(juce::Graphics& g) override;

	void resized() override;



private:
    PresetPanel mPresetPanel;
    ImpulseResponsePanel mIRPanel;

};
