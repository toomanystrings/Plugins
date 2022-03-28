/*
  ==============================================================================

    BDDistortion.cpp
    Created: 29 Apr 2021 3:46:39pm
    Author:  Olorin

  ==============================================================================
*/

#include "Distortion.h"

template <typename T>
void DivisionVoid::Distortion<T>::prepare(double sampleRate, const int& bufferSize)
{
    this->sampleRate = sampleRate;

    tubeScreamer.prepare(sampleRate);
    mxrDistiortion.prepare(sampleRate);
}

template <typename T>
void DivisionVoid::Distortion<T>::process(T* x, const int& channel, const int& bufferSize)
{
    for (int sample = 0; sample < bufferSize; ++sample)
        x[sample] *= inputGainLin;
	

	switch (distortionSetting)
	{
    case 0:
		for (int i = 0; i < bufferSize; ++i)
			x[i] = (1 - mix) * x[i] + mix * tubeScreamer.processSample(x[i], channel);
        break;

    case 1:
		for (int i = 0; i < bufferSize; ++i)
            x[i] = (1 - mix) * x[i] + mix * mxrDistiortion.processSample(x[i], channel);
        break;

        // Confirm the two pedals work, then bring in half rec and maybe something else, just need to confirm the
        // indexing of the knob
    case 2:
        break;
	default:
        for (int i = 0; i < bufferSize; ++i)
            x[i] = (1 - mix) * x[i] + mix * tubeScreamer.processSample(x[i], channel);
        break;
		
	}

    for (int sample = 0; sample < bufferSize; ++sample)
        x[sample] *= outputGainLin;
}

template <typename T>
void DivisionVoid::Distortion<T>::setInputGain(float inputGaindB)
{
    this->inputGainLin = juce::Decibels::decibelsToGain(inputGaindB);
    this->outputGainLin = juce::Decibels::decibelsToGain(-inputGaindB);
}

template <typename T>
float DivisionVoid::Distortion<T>::getInputGaindB()
{
    return juce::Decibels::gainToDecibels(inputGainLin);
}

template <typename T>
void DivisionVoid::Distortion<T>::setEdge(float edge)
{
    this->edge = edge;
    tubeScreamer.setKnob(edge / 10);
    mxrDistiortion.setDrive(edge / 10);
}

template <typename T>
float DivisionVoid::Distortion<T>::getEdge()
{
    return edge;
}

template <typename T>
void DivisionVoid::Distortion<T>::setType(int distortionType)
{
    distortionSetting = distortionType;
}

template <typename T>
int DivisionVoid::Distortion<T>::getType()
{
    return distortionSetting;
}

template <typename T>
void DivisionVoid::Distortion<T>::setMix(float mix)
{
    this->mix = mix / 100;
}

template <typename T>
float DivisionVoid::Distortion<T>::getMix()
{
    return mix;
}