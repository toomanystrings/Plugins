
#pragma once

namespace DivisionVoid
{
class PanelBase : public juce::Component
{
public:
    PanelBase(juce::AudioProcessor& inProcessor) : mProcessor(inProcessor) {}

    ~PanelBase() = default;

    void paint(juce::Graphics& g) override
    {
        g.setColour(DivisionVoidColours::white);
        g.fillAll();

        g.setColour(DivisionVoidColours::black);
        g.drawRect(0, 0, getWidth(), getHeight(), 2);
    }

protected:
    juce::AudioProcessor& mProcessor;

private:
    void initaliseSliders(juce::Slider *sliders,
                          juce::Slider::SliderStyle style)
    {
        int numSliders = sizeof (sliders);

        for (int i = 0; i < numSliders; ++i)
        {
            sliders[i].setSliderStyle(style);
            sliders[i].setTextBoxStyle(juce::Slider::TextBoxBelow, false, 55, 12);
            sliders[i].setTextValueSuffix("dB");
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

    void attachSliders(UniquePtr<juce::AudioProcessorValueTreeState::SliderAttachment> *attachments,
                       juce::AudioProcessor& audioProcessor,
                       juce::String *names,
                       juce::Slider *sliders)
    {
        int numAttachments = sizeof (attachments);

        for (int i = 0; i < numAttachments; ++i)
        {
            //attachments[i] = MakeUnique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            //        audioProcessor.treeState, names[i], sliders[i]);
        }
    }
};
}