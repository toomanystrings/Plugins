/*
  ==============================================================================

    BDAnalyserPanel.h
    Created: 21 Feb 2021 3:58:04pm
    Author:  Olorin

  ==============================================================================
*/

#pragma once

#include "BDPanelBase.h"

#include "BDFftPanel.h"
#include "BDIOPanel.h"
#include "CentreKnobLAF.h"

class BDAnalyserPanel : public BDPanelBase
{
public:
	BDAnalyserPanel(BassDivisionAudioProcessor& inProcessor);
	~BDAnalyserPanel();

	void paint(juce::Graphics& g) override;

	void resized() override;


private:

	CentreKnobLAF LAF;
	
	BDFftPanel mFftPanel;
	BDIOPanel mIoPanel;
};
