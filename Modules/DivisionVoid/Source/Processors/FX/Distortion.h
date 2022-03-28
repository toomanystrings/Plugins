/*
  ==============================================================================

    BDDistortion.h
    Created: 29 Apr 2021 3:46:39pm
    Author:  Olorin

  ==============================================================================
*/

#pragma once
#include "Pedals/TSClipper.h"
#include "Pedals/MXRClipper.h"

namespace DivisionVoid
{
    template <typename T>
	class Distortion
	{
	public:
		Distortion() = default;
		~Distortion() = default;

		void prepare(double sampleRate, const int& bufferSize);

		void process(T* x, const int& channel, const int& bufferSize);

		void setInputGain(float inputGaindB);


		// Returns the gain value in dB.
		float getInputGaindB();

		void setEdge(float edge);

		float getEdge();

		void setType(int distortionType);

		int getType();

		void setMix(float mix);
        float getMix();

	private:
		double sampleRate = 44100;
		int distortionSetting = 1;

		float inputGainLin = 1.0f;
		float outputGainLin = 1.0f;
		float edge = 1.0f;
		float mix = 0.5f;

		TSClipper<T> tubeScreamer;
		MXRClipper<T> mxrDistiortion;
	};
}
