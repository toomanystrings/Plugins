
// A base panel class for use with juce. Currently designed to fit my needs.

#pragma once

#include "JuceHeader.h"
#include "../PluginProcessor.h"
#include "InterfaceDefines.h"

class PanelBase : public DivisionVoid::PanelBase
{
public:
    PanelBase (DivisionVoid::ProcessorBase &inProcessor1, BassDivisionAudioProcessor &inProcessor) : PanelBase(
            inProcessor1), mProcessor(inProcessor)
    {}

	~PanelBase() = default;

    void paint(juce::Graphics& g) override
    {
        g.setColour(DivisionVoid::Colours::white);
        g.fillAll();

        g.setColour(DivisionVoid::Colours::black);
        g.drawRect(0, 0, getWidth(), getHeight(), 2);
    }
    
protected:
    BassDivisionAudioProcessor& mProcessor;

private:
};
