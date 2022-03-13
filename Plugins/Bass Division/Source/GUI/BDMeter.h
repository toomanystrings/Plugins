/*
  ==============================================================================

    BDMeter.h
    Created: 22 Feb 2021 10:33:43pm
    Author:  Olorin

  ==============================================================================
*/

#pragma once

#ifndef JUCE_ENABLE_LIVE_CONSTANT_EDITOR
#define JUCE_ENABLE_LIVE_CONSTANT_EDITOR 0
#endif

#include <JuceHeader.h>

#include "Utilities.h"

// This class will be displaying the values in dB. For this, we need to take into account the logarithmic nature of the values. Any values we pass in will be in gain form, meaning that the markings on the side will have to take these changes into account.

class BDMeter : public juce::Component,
                private juce::Timer
{
public:
    BDMeter()
    {
        startTimerHz(60);
    }
    ~BDMeter() {}
    
    void paint(juce::Graphics& g)
    {
        auto bounds = getLocalBounds();
    }
    
    void resized()
    {
        
    }
    
    
	
private:
    struct OnePoleFilter
    {
        float process(float sample)
        {
            
        }
    }
    
    void timerCallback()
    {
        
    }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BDMeter)
};
