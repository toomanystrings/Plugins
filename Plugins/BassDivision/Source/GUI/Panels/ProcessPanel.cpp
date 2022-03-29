/*
  ==============================================================================

    BDLeftPanel.cpp
    Created: 20 Feb 2021 8:55:39pm
    Author:  Olorin

  ==============================================================================
*/

#include "ProcessPanel.h"



ProcessPanel::ProcessPanel(BassDivisionAudioProcessor& inProcessor)
: PanelBase(inProcessor)//, mEqPanel(inProcessor), mCompPanel(inProcessor)
{
    setSize(PROCESS_PANEL_WIDTH, PROCESS_PANEL_HEIGHT);

    //addAndMakeVisible(mEqPanel);
    //addAndMakeVisible(mCompPanel);
}

ProcessPanel::~ProcessPanel()
{
}

void ProcessPanel::paint(juce::Graphics& g)
{
    PanelBase::paint(g);

    g.setColour(juce::Colours::black);
    g.setFont(25);
    g.drawText("Check the area", 0, 0, getWidth(), getHeight(), juce::Justification::centred);
}

void ProcessPanel::resized()
{
    auto area = getLocalBounds();

    auto eqArea = area.removeFromLeft(PROCESS_PANEL_WIDTH / 2);
    //mEqPanel.setBounds(eqArea);
    //mCompPanel.setBounds(area);
}
