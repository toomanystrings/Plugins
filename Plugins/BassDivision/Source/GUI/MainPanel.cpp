/*
  ==============================================================================

    BDMainPanel.cpp
    Created: 20 Feb 2021 9:08:16pm
    Author:  Olorin

  ==============================================================================
*/

#include "MainPanel.h"

MainPanel::MainPanel(BassDivisionAudioProcessor& inProcessor) : PanelBase(inProcessor),
topPanel(inProcessor), middlePanel(inProcessor), bottomPanel(inProcessor)
{
    setSize(MAIN_PANEL_WIDTH, MAIN_PANEL_HEIGHT);

    addAndMakeVisible(middlePanel);
    addAndMakeVisible(bottomPanel);
    addAndMakeVisible(topPanel);
}

MainPanel::~MainPanel()
{
}

void MainPanel::paint(juce::Graphics& g)
{
    PanelBase::paint(g);
}

void MainPanel::resized()
{
    auto area = getLocalBounds();
    //auto height = area.getHeight();

    //const float topPanelRatio = TOP_PANEL_HEIGHT / MAIN_PANEL_HEIGHT;
    //auto analyserPanelRatio = area.removeFromTop(ANALYSER_PANEL_HEIGHT);

    
    topPanel.setBounds(area.removeFromTop(TOP_PANEL_HEIGHT));
   
    //auto bottomArea = area.removeFromBottom(PROCESS_PANEL_HEIGHT);
	
    //mBottomPanel.setBounds(bottomArea);

    //mAnalyserPanel.setBounds(area);

    bottomPanel.setBounds(area.removeFromBottom(PROCESS_PANEL_HEIGHT));
    middlePanel.setBounds(area);
}
