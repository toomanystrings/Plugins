/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MPESynthAudioProcessorEditor::MPESynthAudioProcessorEditor (MPESynthAudioProcessor& p)
        : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (600, 600);
}

MPESynthAudioProcessorEditor::~MPESynthAudioProcessorEditor()
{
}

//==============================================================================
void MPESynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.setColour(juce::Colours::white);
    g.fillAll();

    g.setColour(juce::Colours::black);
    //g.setFont(15);
    //g.drawText("MPE Synth", getLocalBounds(), juce::Justification::centred);

    // Generic useful bounds variables
    auto width = getWidth();
    auto halfWidth = width / 2;
    auto height = getHeight();
    auto halfHeight = height / 2;
    juce::Point<int> centre {width / 2, height / 2};

    auto bezScaler = 1.25f;

    auto radius = 200;

    //g.fillEllipse(centre.getX() - 3, centre.getY() - 3, 6, 6);

    g.drawLine(width / 2, 0, width / 2, height, 2);
    g.drawLine(0, height / 2, width, height / 2, 2);

    juce::Point<float> start {centre.getX() + radius * cosf(0), centre.getY() + radius * -sinf(0)};
    juce::Point<float> end {centre.getX() + radius * cosf(pi / 2), centre.getY() + radius * -sinf(pi / 2)};

    //juce::Point<float> q {start.getX(), end.getY()};
    juce::Point<float> q {centre.getX() + radius * bezScaler * cosf(pi / 4),
                          centre.getY() + radius * bezScaler * -sinf(pi / 4)};

    g.fillEllipse(q.getX() - 4, q.getY() - 4, 8, 8);

    juce::Path path;

    path.startNewSubPath(start);
    path.quadraticTo(q, end);

    g.strokePath(path, juce::PathStrokeType{2});

}

void MPESynthAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor...
}
