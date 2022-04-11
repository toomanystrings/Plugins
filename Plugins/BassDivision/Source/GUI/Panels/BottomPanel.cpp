/*
  ==============================================================================

    BDBottomPanel.cpp
    Created: 5 May 2021 3:30:01pm
    Author:  Olorin

  ==============================================================================
*/

#include "BottomPanel.h"

// Bottom Panel

BottomPanel::BottomPanel(BassDivisionAudioProcessor& inProcessor) : PanelBase(inProcessor), mProcessPanel(inProcessor), mBandPanel(inProcessor)
{
    setSize(PROCESS_PANEL_WIDTH, PROCESS_PANEL_HEIGHT);

    addAndMakeVisible(mProcessPanel);
    addAndMakeVisible(mBandPanel);
    mBandPanel.setVisible(false);

    processButton.setButtonStyle(DivisionVoid::Button::BarToggle);
    processButton.setButtonText("Process");
    processButton.setToggleState(true, juce::NotificationType::dontSendNotification);
    processButton.onClick = [this] ()
    {
        if (processButton.getToggleState() == true)
        {
            processButton.setToggleState(true, juce::NotificationType::dontSendNotification);
            bandButton.setToggleState(false, juce::NotificationType::dontSendNotification);
            mProcessPanel.setVisible(true);
            mBandPanel.setVisible(false);
        }

		if (processButton.getToggleState() == false)
		{
            processButton.setToggleState(true, juce::NotificationType::dontSendNotification);
		}
    };
    addAndMakeVisible(processButton);

    bandButton.setButtonStyle(DivisionVoid::Button::BarToggle);
    bandButton.setButtonText("Band");
    bandButton.onClick = [this] ()
    {
        if (bandButton.getToggleState() == true)
        {
            bandButton.setToggleState(true, juce::NotificationType::dontSendNotification);
            processButton.setToggleState(false, juce::NotificationType::dontSendNotification);
            mProcessPanel.setVisible(false);
            mBandPanel.setVisible(true);
        }

        if (bandButton.getToggleState() == false)
        {
            bandButton.setToggleState(true, juce::NotificationType::dontSendNotification);
        }
    };
    addAndMakeVisible(bandButton);
}

BottomPanel::~BottomPanel()
{
}

void BottomPanel::paint(juce::Graphics& g)
{
	
}

void BottomPanel::resized()
{
    auto xPivot = 10;
	
    processButton.setBounds(xPivot, getHeight() / 2 - 25, 120, 25);
    processButton.setTransform(juce::AffineTransform::rotation(M_PI_2 * 3, xPivot, getHeight() / 2 - 25));

    bandButton.setBounds(xPivot, getHeight() - 55, 120, 25);
    bandButton.setTransform(juce::AffineTransform::rotation(M_PI_2 * 3, xPivot, getHeight() - 55));

	processButton.setFont(fonts.getFont(DivisionVoidFonts::FontType::bold, 17));
	bandButton.setFont(fonts.getFont(DivisionVoidFonts::FontType::bold, 17));
}


// Process Panel

ProcessPanel::ProcessPanel(BassDivisionAudioProcessor& inProcessor) :
    PanelBase(inProcessor), eqPanel(inProcessor), compressorPanel(inProcessor)
{
    setSize(PROCESS_PANEL_WIDTH, PROCESS_PANEL_HEIGHT);

    addAndMakeVisible(eqPanel);
    addAndMakeVisible(compressorPanel);
}

void ProcessPanel::paint(juce::Graphics& g)
{

}

void ProcessPanel::resized()
{
    auto area = getLocalBounds();

    auto eqArea = area.removeFromLeft(PROCESS_PANEL_WIDTH / 2);
    eqPanel.setBounds(eqArea);
    compressorPanel.setBounds(area);
}

ProcessPanel::EqPanel::EqPanel(BassDivisionAudioProcessor &inProcessor) : PanelBase(inProcessor)
{

}

void ProcessPanel::EqPanel::paint(juce::Graphics &g)
{
    g.setFont(fonts.getFont(DivisionVoidFonts::FontType::bold, 25.0f));
    g.drawText("EQUALISER", 0, 0, getWidth() - 15, getHeight() - 15, juce::Justification::centredBottom);
}

void ProcessPanel::EqPanel::resized()
{

}

ProcessPanel::CompressorPanel::CompressorPanel(BassDivisionAudioProcessor &inProcessor) : PanelBase(inProcessor)
{
    // "ATTACK", "RELEASE", "RATIO", "THRESHOLD",
    // "KNEE", "HPF", "INPUT", "OUTPUT"
    for (int i = 0; i < 8; ++i)
    {
        sliders[i].setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
        sliders[i].setTextBoxStyle(juce::Slider::TextBoxBelow, false, 55, 12);

        labels[i].setText(names[i], juce::dontSendNotification);
        labels[i].attachToComponent(&sliders[i], false);
        labels[i].setJustificationType(juce::Justification::centred);
        labels[i].setColour(0x1000281, juce::Colours::black);

        attachments[i] = MakeUnique<juce::AudioProcessorValueTreeState::SliderAttachment>(
                inProcessor.treeState, "COMP_" + names[i], sliders[i]
                );
    }

    for (auto &slider : sliders)
    {
        addAndMakeVisible(slider);
    }

    for (auto &label : labels)
    {
        addAndMakeVisible(label);
    }


//    sliders[0].setTextValueSuffix("ms");
//    sliders[0].setLookAndFeel(&LAF);

//    sliders[1].setTextValueSuffix("ms");
//    sliders[1].setLookAndFeel(&LAF);

//    sliders[2].setLookAndFeel(&LAF);

//    sliders[3].setTextValueSuffix("dB");
//    sliders[3].setLookAndFeel(&LAF);

//    sliders[4].setLookAndFeel(&LAF);

//    sliders[5].setTextValueSuffix("Hz");
//    sliders[5].setLookAndFeel(&LAF);

//    sliders[6].setTextValueSuffix("dB");
//    sliders[6].setLookAndFeel(&centreLAF);

//    sliders[7].setTextValueSuffix("dB");
//    sliders[7].setLookAndFeel(&centreLAF);
}

void ProcessPanel::CompressorPanel::paint(juce::Graphics &g)
{
    g.setFont(fonts.getFont(DivisionVoidFonts::FontType::bold, 25.0f));
    g.drawText("COMPRESSOR", 0, 0, getWidth() - 15, getHeight() - 15, juce::Justification::centredBottom);
}

void ProcessPanel::CompressorPanel::resized()
{
    // Basic calculations for the layout. Realistically this should be dynamic, in a grid layout or using methods provided by juce, but this
    //should work for now
    const auto width = getWidth();
    const auto height = getHeight();

    const auto centreY = height / 2;

    const auto knobDiameter = height / 4;

    const auto border = 20;
    const auto yOffset = 30;

    // Threshold Ratio Attack Release
    // Input Knee HPF Output

    sliders[3].setBounds((width / 6 * 1), border + yOffset, knobDiameter, knobDiameter);
    sliders[2].setBounds((width / 6 * 2), border + yOffset, knobDiameter, knobDiameter);
    sliders[0].setBounds((width / 6 * 3), border + yOffset, knobDiameter, knobDiameter);
    sliders[1].setBounds((width / 6 * 4), border + yOffset, knobDiameter, knobDiameter);

    sliders[6].setBounds((width / 6 * 1), border + centreY, knobDiameter, knobDiameter);
    sliders[4].setBounds((width / 6 * 2), border + centreY, knobDiameter, knobDiameter);
    sliders[5].setBounds((width / 6 * 3), border + centreY, knobDiameter, knobDiameter);
    sliders[7].setBounds((width / 6 * 4), border + centreY, knobDiameter, knobDiameter);

    for (auto& label : labels)
    {
        label.setFont(fonts.getFont(DivisionVoidFonts::FontType::bold, 17));
    }
}


// Band Panel

BandPanel::BandPanel(BassDivisionAudioProcessor &inProcessor) : PanelBase(inProcessor)
{

}

void BandPanel::paint(juce::Graphics& g)
{

}

void BandPanel::resized()
{

}