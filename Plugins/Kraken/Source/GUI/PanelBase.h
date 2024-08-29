//
// Created by Conor Foran on 17/07/2024.
//


#pragma once

#include <JuceHeader.h>
#include "../PluginProcessor.h"

namespace Kraken
{
class PanelBase : public juce::Component
{
public:
    PanelBase(KrakenAudioProcessor& inProcessor) : audioProcessor(inProcessor) {}

    ~PanelBase() = default;

    void paint(juce::Graphics& g) override
    {
        // Here is where we put the default stuff for a panel, in terms of colour and what have you.
    }


private:
    KrakenAudioProcessor& audioProcessor;
};
}
