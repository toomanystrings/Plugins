/*
  ==============================================================================

    MeterAnalysis.h
    Created: 9 Jun 2021 5:08:35pm
    Author:  Olorin

  ==============================================================================
*/

#include <JuceHeader.h>
#include "../FIFO.h"

#pragma once
class MeterAnalysis : public juce::TimeSliceClient
{
public:
    MeterAnalysis(int size);

	~MeterAnalysis();

    void setSampleRate(double sampleRate);

    void copySamples(std::vector<float>& array, bool isPlaying);

    virtual int useTimeSlice() override;

    void process();

    juce::Value& getRMSValue();
    juce::Value& getRepaintViewer();
    juce::Value& getIsPlaying();

private:
    double sampleRate;
    DivisionVoid::FIFO fifo;

    juce::Value RMSValue;
    juce::Value repaintViewer;
    juce::Value isPlaying;

    float previousSample[2] = { 0.0f };

    float ga;
    float gr;
    float g;

    //float rootTwo = 1.0f / sqrt(2);

    bool needToProcess;
    bool isGainReduction;
};
