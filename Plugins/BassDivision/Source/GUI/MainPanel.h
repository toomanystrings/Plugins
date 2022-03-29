/*
  ==============================================================================

    BDMainPanel.h
    Created: 20 Feb 2021 9:08:16pm
    Author:  Olorin

  ==============================================================================
*/

#pragma once

#include "PanelBase.h"
//#include "../../../Bass Division/Source/GUI/BDTopPanel.h"
#include "Panels/BottomPanel.h"


//#include "../../../Bass Division/Source/GUI/BDAnalyserPanel.h"

class MainPanel : public PanelBase
{
public:
    MainPanel(BassDivisionAudioProcessor& inProcessor);
	~MainPanel();

    void paint(juce::Graphics& g) override;

	void resized() override;
	
private:

    //BDTopPanel mTopPanel;
    //BDAnalyserPanel mAnalyserPanel;
    BottomPanel mBottomPanel;
};
