
// A base panel class for use with juce. Currently designed to fit my needs.

#pragma once

#include "JuceHeader.h"
#include "../PluginProcessor.h"
#include "InterfaceDefines.h"

class PanelBase : public juce::Component
{
public:
    PanelBase(BassDivisionAudioProcessor& inProcessor) : mProcessor(inProcessor) {}

	~PanelBase() = default;

    void paint(juce::Graphics& g) override
    {
        g.setColour(DivisionVoidColours::white);
        g.fillAll();

        g.setColour(DivisionVoidColours::black);
        g.drawRect(0, 0, getWidth(), getHeight(), 2);
    }
    
protected:
    BassDivisionAudioProcessor& mProcessor;

private:
    void initaliseSliders(juce::Slider *sliders, juce::Label *labels, juce::Slider::SliderStyle style)
    {

    }


};
