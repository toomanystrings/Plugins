/*
  ==============================================================================

    BDBottomPanel.cpp
    Created: 5 May 2021 3:30:01pm
    Author:  Olorin

  ==============================================================================
*/

#include "BDBottomPanel.h"

BDBottomPanel::BDBottomPanel(BassDivisionAudioProcessor& inProcessor) : BDPanelBase(inProcessor), mProcessPanel(inProcessor), mBandPanel(inProcessor)
{
    setSize(PROCESS_PANEL_WIDTH, PROCESS_PANEL_HEIGHT);

    //mProcessPanel = new BDProcessPanel(inProcessor);
    addAndMakeVisible(mProcessPanel);
	

    //mBandPanel = new BDBandPanel(inProcessor);
    addAndMakeVisible(mBandPanel);
    mBandPanel.setVisible(false);

	/// TODO: Add in the band processor, and add in button code to switch between the views {BIG FUCKING TICK}
	
    //norse.setBold(true);

    processButton.addListener(this);
    //processButton.setFont(norse);
    processButton.setButtonStyle(DivisionVoid::Button::BarToggle);
    processButton.setButtonText("Process");
    processButton.setToggleState(true, juce::NotificationType::dontSendNotification);
    addAndMakeVisible(processButton);

    bandButton.addListener(this);
    //bandButton.setFont(norse);
    bandButton.setButtonStyle(DivisionVoid::Button::BarToggle);
    bandButton.setButtonText("Band");
    addAndMakeVisible(bandButton);
}

BDBottomPanel::~BDBottomPanel()
{
}

void BDBottomPanel::paint(juce::Graphics& g)
{
	
}

void BDBottomPanel::resized()
{
    auto xPivot = 10;
	
    processButton.setBounds(xPivot, getHeight() / 2 - 25, 120, 25);
    processButton.setTransform(juce::AffineTransform::rotation(M_PI_2 * 3, xPivot, getHeight() / 2 - 25));

    bandButton.setBounds(xPivot, getHeight() - 55, 120, 25);
    bandButton.setTransform(juce::AffineTransform::rotation(M_PI_2 * 3, xPivot, getHeight() - 55));

	processButton.setFont(DVFonts::getBoldNorse(17));
	bandButton.setFont(DVFonts::getBoldNorse(17));
}

void BDBottomPanel::buttonClicked(juce::Button* b)
{
	if (b == &processButton)
	{
        if (processButton.getToggleState() == true)
        {
            processButton.setToggleState(true, juce::NotificationType::dontSendNotification);
            bandButton.setToggleState(false, juce::NotificationType::dontSendNotification);
            mBandPanel.setVisible(false);
            mProcessPanel.setVisible(true);
        }

		if (processButton.getToggleState() == false)
		{
            processButton.setToggleState(true, juce::NotificationType::dontSendNotification);
		}
	}

	if (b == &bandButton)
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
	}
}


