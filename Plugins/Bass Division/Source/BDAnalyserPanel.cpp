/*
  ==============================================================================

    BDAnalyserPanel.cpp
    Created: 21 Feb 2021 3:58:04pm
    Author:  Olorin

  ==============================================================================
*/

#include "BDAnalyserPanel.h"

BDAnalyserPanel::BDAnalyserPanel(BassDivisionAudioProcessor& inProcessor) : BDPanelBase(inProcessor), mFftPanel(inProcessor, inProcessor.getSpectrumProcessor()->getRepaintViewerValue(),inProcessor.getSpectrumProcessor()->getMagnitudesBuffer(), inProcessor.getSpectrumProcessor()->getDetectedFrequency()), mIoPanel(inProcessor)
{
    mFftPanel.setEnabled(true);
    addAndMakeVisible(mFftPanel);
    addAndMakeVisible(mIoPanel);
}

BDAnalyserPanel::~BDAnalyserPanel()
{
}

void BDAnalyserPanel::paint(juce::Graphics& g)
{
    BDPanelBase::paint(g);
}

void BDAnalyserPanel::resized()
{
    auto area = getLocalBounds();
    //auto height = getHeight();
    

    /*if (height < ANALYSER_PANEL_HEIGHT / 2.7)
    {
        mFftPanel.setEnabled(false);
        mFftPanel.setVisible(false);
    }
    else if (height < ANALYSER_PANEL_HEIGHT / 1.8)
    {
        auto fftProportion = height * 2 / 4;
        mFftPanel.setBounds(area.removeFromTop(fftProportion));
        mFftPanel.setVisible(true);
        
    }
    else
    {
        auto fftProportion = height * 3 / 4;
        mFftPanel.setBounds(area.removeFromTop(fftProportion));
        mFftPanel.setVisible(true);
        
    }*/

    mIoPanel.setBounds(area.removeFromRight(IO_PANEL_WIDTH));
    mFftPanel.setBounds(area);
    //mIoPanel.setBounds(area.removeFromLeft(DIST_PANEL_WIDTH));
    
}
