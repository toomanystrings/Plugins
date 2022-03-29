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

}

void ProcessPanel::CompressorPanel::paint(juce::Graphics &g)
{
    g.setFont(fonts.getFont(DivisionVoidFonts::FontType::bold, 25.0f));
    g.drawText("COMPRESSOR", 0, 0, getWidth() - 15, getHeight() - 15, juce::Justification::centredBottom);
}

void ProcessPanel::CompressorPanel::resized()
{

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