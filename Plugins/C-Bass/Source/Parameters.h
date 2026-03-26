//
// Created by Conor Foran on 26/03/2026.
//

#pragma once

#include <JuceHeader.h>

struct Parameters
{
    void add(juce::AudioProcessor& processor)
    {
        // processor.addParameter(gain)
    }

    // Here we add raw pointers i.e
    // juce::AudioParameterFloat* gain =
    //        new juce::AudioParameterFloat({"Gain", 1}, "Gain", 0.f, 1.f, 0.5f);
};
