/*
  ==============================================================================

    BDTopPanel.cpp
    Created: 20 Feb 2021 8:53:19pm
    Author:  Olorin

  ==============================================================================
*/

#include "BDTopPanel.h"

BDTopPanel::BDTopPanel(BassDivisionAudioProcessor& inProcessor) : BDPanelBase(inProcessor), mPresetPanel(inProcessor), mIRPanel(inProcessor)
{
    //setSize(TOP_PANEL_WIDTH, TOP_PANEL_HEIGHT);

    addAndMakeVisible(mPresetPanel);
    addAndMakeVisible(mIRPanel);
}

BDTopPanel::~BDTopPanel()
{
}

void BDTopPanel::paint(juce::Graphics& g)
{
    //BDPanelBase::paint(g);
}

void BDTopPanel::resized()
{
    auto area = getLocalBounds();
	auto halfWidth = getWidth() / 2;

    mIRPanel.setBounds(halfWidth, 0, halfWidth - 1, getHeight());
    mPresetPanel.setBounds(0, 0, halfWidth, getHeight());
    
	
}
