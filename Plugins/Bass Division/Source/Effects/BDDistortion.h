/*
  ==============================================================================

    BDDistortion.h
    Created: 29 Apr 2021 3:46:39pm
    Author:  Olorin

  ==============================================================================
*/

#pragma once
#include "TSClipper.h"
#include "MXRClipper.h"

namespace DivisionVoid
{
	class Distortion
	{
	public:
		Distortion();
		~Distortion();

		void prepare(double sampleRate, const int& bufferSize);

		void process(float* x, const int& channel, const int& bufferSize);

		void setInputGain(float inputGaindB)
		{
			this->inputGainLin = juce::Decibels::decibelsToGain(inputGaindB);
			this->outputGainLin = juce::Decibels::decibelsToGain(-inputGaindB);
		}

		// Returns the gain value in dB.
		float getInputGaindB()
		{
			return juce::Decibels::gainToDecibels(inputGainLin);
		}

		void setEdge(float edge)
		{
			this->edge = edge;
			tubeScreamer.setKnob(edge / 10);
			mxrDistiortion.setDrive(edge / 10);
		}

		float getEdge()
		{
			return edge;
		}

		void setType(int distortionType)
		{
			this->distortionSetting = distortionType;
		}

		int getType()
		{
			return distortionSetting;
		}

		void setMix(float mix)
		{
			this->mix = mix / 100;
		}

	private:
		double sampleRate = 44100;
		int distortionSetting = 1;

		float inputGainLin = 1.0f;
		float outputGainLin = 1.0f;
		float edge = 1.0f;
		float mix = 0.5f;

		TSClipper tubeScreamer;
		MXRClipper mxrDistiortion;
	};
}
