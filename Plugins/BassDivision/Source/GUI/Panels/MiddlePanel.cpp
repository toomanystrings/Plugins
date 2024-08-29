//
// Created by Conor on 11/04/2022.
//

#include "MiddlePanel.h"

MiddlePanel::MiddlePanel(BassDivisionAudioProcessor &inProcessor) :
        PanelBase(inProcessor),
        ioPanel(inProcessor)//,
        //fftPanel(inProcessor, inProcessor.getSpectrumProcessor()->getRepaintViewerValue(),
        //         inProcessor.getSpectrumProcessor()->getMagnitudesBuffer(),
        //         inProcessor.getSpectrumProcessor()->getDetectedFrequency())
{
    addAndMakeVisible(ioPanel);
    //addAndMakeVisible(fftPanel);
}

void MiddlePanel::paint(juce::Graphics &g)
{

}

void MiddlePanel::resized()
{
    auto area = getLocalBounds();

    ioPanel.setBounds(area.removeFromRight(IO_PANEL_WIDTH));
    //fftPanel.setBounds(area);
}

IoPanel::IoPanel(BassDivisionAudioProcessor &inProcessor) : PanelBase(inProcessor)
{
    //setSize(IO_PANEL_WIDTH, IO_PANEL_HEIGHT);
    for (int i = 0; i < 2; ++i)
    {
        sliders[i].setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
        sliders[i].setTextBoxStyle(juce::Slider::TextBoxBelow, false, 55, 12);
        sliders[i].setTextValueSuffix("dB");
        sliders[i].setLookAndFeel(&LAF);
        addAndMakeVisible(&sliders[i]);

        labels[i].setText(names[i], juce::dontSendNotification);
        labels[i].attachToComponent(&sliders[i], false);
        labels[i].setJustificationType(juce::Justification::centred);
        labels[i].setColour(0x1000281, juce::Colours::black);
        addAndMakeVisible(labels[i]);

        attachments[i] = MakeUnique<juce::AudioProcessorValueTreeState::SliderAttachment>(
                inProcessor.treeState, names[i], sliders[i]
                );
    }
}

void IoPanel::paint(juce::Graphics &g)
{
}

void IoPanel::resized()
{
    auto bounds = getBounds();

    auto border = 25;

    const auto width = getWidth();
    const auto height = getHeight();

    const auto centreX = width / 2;
    const auto centreY = height / 2;

    const auto gainSliderSize = 40;

    const auto gainY = 25;

    sliders[0].setBounds(centreX - gainSliderSize - 5, 30, gainSliderSize, 260);
    sliders[1].setBounds(centreX + 5, 30, gainSliderSize, 260);

    for (auto& label : labels) {
        label.setFont(fonts.getFont(DivisionVoidFonts::FontType::bold, 17));
    }
}

/*FftPanel::FftPanel(BassDivisionAudioProcessor &inProcessor, juce::Value &repaintSpectrumViewer,
                   drow::Buffer &spectrumMagnitudeBuffer, juce::Value &detectedFrequency) :
                   PanelBase(inProcessor),
                   spectrumViewer(repaintSpectrumViewer, spectrumMagnitudeBuffer, detectedFrequency)
{
    addAndMakeVisible(spectrumViewer);
}

void FftPanel::paint(juce::Graphics &g)
{
}

void FftPanel::resized()
{
    spectrumViewer.setBounds(getLocalBounds());
}*/
