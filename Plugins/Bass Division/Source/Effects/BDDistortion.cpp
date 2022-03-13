/*
  ==============================================================================

    BDDistortion.cpp
    Created: 29 Apr 2021 3:46:39pm
    Author:  Olorin

  ==============================================================================
*/

#include "BDDistortion.h"

DivisionVoid::Distortion::Distortion()
{
}

DivisionVoid::Distortion::~Distortion()
{
}

void DivisionVoid::Distortion::prepare(double sampleRate, const int& bufferSize)
{
    this->sampleRate = sampleRate;

    tubeScreamer.prepare(sampleRate);
    mxrDistiortion.prepare(sampleRate);
}

void DivisionVoid::Distortion::process(float* x, const int& channel, const int& bufferSize)
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

        // Confirm the two pedals work, then bring in half rec and maybe something else, just need to confirm the indexing of the knob
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
