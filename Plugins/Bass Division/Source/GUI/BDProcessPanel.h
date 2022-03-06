/*
  ==============================================================================

    BDLeftPanel.h
    Created: 20 Feb 2021 8:55:39pm
    Author:  Olorin

  ==============================================================================
*/

#pragma once

#include "BDPanelBase.h"
#include "BDEqPanel.h"
#include "BDCompPanel.h"


class BDProcessPanel : public BDPanelBase
{
public:
    BDProcessPanel(BassDivisionAudioProcessor& inProcessor);
    ~BDProcessPanel();

    void paint(juce::Graphics& g) override;

    void resized() override;


private:

    BDEqPanel mEqPanel;
    BDCompPanel mCompPanel;

    
    
};
