/*
  ==============================================================================

    BDPanelBase.h
    Created: 20 Feb 2021 8:52:32pm
    Author:  Olorin

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../PluginProcessor.h"
#include "InterfaceDefines.h"

class BDPanelBase : public juce::Component
{
public:
    BDPanelBase(BassDivisionAudioProcessor& inProcessor);
	~BDPanelBase();

    void paint(juce::Graphics& g) override;
    
protected:
    BassDivisionAudioProcessor& mProcessor;
};
