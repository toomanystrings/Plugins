/*
  ==============================================================================

    BDBandPanel.h
    Created: 20 Feb 2021 8:53:53pm
    Author:  Olorin

  ==============================================================================
*/

#include "BDPanelBase.h"
#include "SubGUI.h"
#include "LowGUI.h"
#include "MidGUI.h"
#include "HighGUI.h"
#include "LowCrossover.h"
#include "MidCrossover.h"
#include "HighCrossover.h"

#pragma once
class BDBandPanel : public BDPanelBase
{
public:
    BDBandPanel(BassDivisionAudioProcessor& inProcessor);
	~BDBandPanel();

	void paint(juce::Graphics& g) override;

	void resized() override;


private:

    SubGUI mSubGui;
    LowGUI mLowGui;
    MidGUI mMidGui;
    HighGUI mHighGui;

    LowCrossover mLowCrossover;
    MidCrossover mMidCrossover;
    HighCrossover mHighCrossover;
};
