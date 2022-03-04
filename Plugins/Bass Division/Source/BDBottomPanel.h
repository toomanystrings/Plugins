/*
  ==============================================================================

    BDBottomPanel.h
    Created: 5 May 2021 3:30:01pm
    Author:  Olorin

  ==============================================================================
*/

#include "BDPanelBase.h"
#include "Utilities.h"

#include "BDProcessPanel.h"
#include "BDBandPanel.h"

#pragma once
class BDBottomPanel : public BDPanelBase, private juce::Button::Listener
{
public:
    BDBottomPanel(BassDivisionAudioProcessor& inProcessor);
	~BDBottomPanel();

	void paint(juce::Graphics& g) override;

	void resized() override;

	void buttonClicked(juce::Button* button) override;

private:
	BDProcessPanel mProcessPanel;
	BDBandPanel mBandPanel;

	//juce::ScopedPointer<BDProcessPanel> mProcessPanel;
	//juce::ScopedPointer<BDBandPanel> mBandPanel;
	
	DivisionVoid::Button processButton;
	DivisionVoid::Button bandButton;
	
	//juce::Button processButton;
};
