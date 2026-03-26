
#pragma once

#include "../processors/ProcessorBase.h"

namespace DivisionVoid
{
class PanelBase : public juce::Component
{
public:
    PanelBase() = default;
    PanelBase(ProcessorBase& inProcessor) : mProcessor(inProcessor) {}

    ~PanelBase() = default;

    void paint(juce::Graphics& g) override
    {
        g.setColour(DivisionVoid::Colours::white);
        g.fillAll();

        g.setColour(DivisionVoid::Colours::red);
        g.setFont(13);
        g.drawText("Default UI Panel", getBounds(), juce::Justification::centred);

        g.setColour(DivisionVoid::Colours::black);
        g.drawRect(0, 0, getWidth(), getHeight(), 2);
    }

    void initaliseSliders(juce::Slider *sliders,
                          juce::Slider::SliderStyle style,
                          juce::String textValueSuffix)
    {
        int numSliders = sizeof (sliders);

        for (int i = 0; i < numSliders; ++i)
        {
            sliders[i].setSliderStyle(style);
            sliders[i].setTextBoxStyle(juce::Slider::TextBoxBelow, false, 55, 12);
            sliders[i].setTextValueSuffix(textValueSuffix);
            //sliders[i].setLookAndFeel(&centreLAF);
            addAndMakeVisible(&sliders[i]);
        }
    }

    void initaliseLabels(juce::Label *labels,
                         juce::Slider *sliders,
                         juce::String *text,
                         juce::Justification justification)
    {
        int numLabels = sizeof (labels);

        for (int i = 0; i < numLabels; ++i)
        {
            labels[i].setText(text[i], juce::dontSendNotification);
            labels[i].attachToComponent(&sliders[i], false);
            labels[i].setJustificationType(justification);
            labels[i].setColour(0x1000281, juce::Colours::black);
            addAndMakeVisible(labels[i]);
        }
    }

    void attachSliders(ProcessorBase& processor,
                       UniquePtr<juce::AudioProcessorValueTreeState::SliderAttachment> *attachments,
                       juce::String *names,
                       juce::Slider *sliders)
    {
        jassert(sizeof (attachments) == sizeof (names) == sizeof (sliders));
        int numAttachments = sizeof (attachments);

        for (int i = 0; i < numAttachments; ++i)
        {
            //attachments[i] = MakeUnique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            //        processor.treeState, names[i], sliders[i]);
        }
    }

private:
    ProcessorBase& mProcessor;
};
}