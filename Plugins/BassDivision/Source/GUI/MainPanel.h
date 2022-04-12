/*
  ==============================================================================

    BDMainPanel.h
    Created: 20 Feb 2021 9:08:16pm
    Author:  Olorin

  ==============================================================================
*/

#pragma once

#include "PanelBase.h"
#include "Panels/BottomPanel.h"
#include "Panels/MiddlePanel.h"
#include "Panels/TopPanel.h"

class MainPanel : public PanelBase
{
public:
    MainPanel(BassDivisionAudioProcessor& inProcessor);
	~MainPanel();

    void paint(juce::Graphics& g) override;

	void resized() override;
	
private:

    TopPanel topPanel;
    MiddlePanel middlePanel;
    BottomPanel bottomPanel;
};
