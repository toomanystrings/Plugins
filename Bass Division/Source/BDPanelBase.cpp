/*
  ==============================================================================

    BDPanelBase.cpp
    Created: 20 Feb 2021 8:52:32pm
    Author:  Olorin

  ==============================================================================
*/

#include "BDPanelBase.h"

BDPanelBase::BDPanelBase(BassDivisionAudioProcessor& inProcessor) : mProcessor(inProcessor)
{
}

BDPanelBase::~BDPanelBase()
{
}

void BDPanelBase::paint(juce::Graphics& g)
{
    g.setColour(juce::Colours::whitesmoke);
    g.fillAll();

    g.setColour(juce::Colours::black);
    g.drawRect(0, 0, getWidth(), getHeight(), 2);
}


