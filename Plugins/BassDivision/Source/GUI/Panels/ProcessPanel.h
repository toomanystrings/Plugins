/*
  ==============================================================================

    BDLeftPanel.h
    Created: 20 Feb 2021 8:55:39pm
    Author:  Olorin

  ==============================================================================
*/

#pragma once

#include "../PanelBase.h"
//#include "../../../../Bass Division/Source/GUI/BDEqPanel.h"
//#include "../../../../Bass Division/Source/GUI/BDCompPanel.h"


class ProcessPanel : public PanelBase
{
public:
    ProcessPanel(BassDivisionAudioProcessor& inProcessor);
    ~ProcessPanel();

    void paint(juce::Graphics& g) override;

    void resized() override;


private:

    //BDEqPanel mEqPanel;
    //BDCompPanel mCompPanel;

    
    
};
