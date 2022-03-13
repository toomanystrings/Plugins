/*
  ==============================================================================

    BDMainPanel.h
    Created: 20 Feb 2021 9:08:16pm
    Author:  Olorin

  ==============================================================================
*/

#pragma once

#include "BDPanelBase.h"
#include "BDTopPanel.h"
#include "BDBottomPanel.h"


#include "BDAnalyserPanel.h"

class BDMainPanel : public BDPanelBase
{
public:
    BDMainPanel(BassDivisionAudioProcessor& inProcessor);
	~BDMainPanel();

    void paint(juce::Graphics& g) override;

	void resized() override;
	
private:

    BDTopPanel mTopPanel;
    BDAnalyserPanel mAnalyserPanel;
    BDBottomPanel mBottomPanel;
    

    
};
