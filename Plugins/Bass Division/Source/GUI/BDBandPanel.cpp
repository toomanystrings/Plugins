/*
  ==============================================================================

    BDBandPanel.cpp
    Created: 20 Feb 2021 8:53:53pm
    Author:  Olorin

  ==============================================================================
*/

#include "BDBandPanel.h"

BDBandPanel::BDBandPanel(BassDivisionAudioProcessor& inProcessor) : BDPanelBase(inProcessor), mSubGui(inProcessor), mLowGui(inProcessor), mMidGui(inProcessor), mHighGui(inProcessor),mLowCrossover(inProcessor), mMidCrossover(inProcessor), mHighCrossover(inProcessor)
{
    setSize(PROCESS_PANEL_WIDTH, PROCESS_PANEL_HEIGHT);
	
    addAndMakeVisible(mSubGui);
    addAndMakeVisible(mLowGui);
    addAndMakeVisible(mMidGui);
    addAndMakeVisible(mHighGui);

    addAndMakeVisible(mLowCrossover);
    addAndMakeVisible(mMidCrossover);
    addAndMakeVisible(mHighCrossover);
}

BDBandPanel::~BDBandPanel()
{
}

void BDBandPanel::paint(juce::Graphics& g)
{
}

void BDBandPanel::resized()
{
    auto area = getBounds();

    area.removeFromRight(20);
    mHighGui.setBounds(area.removeFromRight(DIST_PANEL_WIDTH)));
	mHighCrossover.setBounds(area.removeFromRight(XOVER_PANEL_WIDTH));
    mMidGui.setBounds(area.removeFromRight(DIST_PANEL_WIDTH)));
    mMidCrossover.setBounds(area.removeFromRight(XOVER_PANEL_WIDTH));
    mLowGui.setBounds(area.removeFromRight(COMP_PANEL_WIDTH)));
    mLowCrossover.setBounds(area.removeFromRight(XOVER_PANEL_WIDTH));
    mSubGui.setBounds(area.removeFromRight(COMP_PANEL_WIDTH)));
    
    
    
}
