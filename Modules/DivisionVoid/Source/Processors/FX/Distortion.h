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

		void prepare(double sampleRate, const int& bufferSize)
        {
            this->sampleRate = sampleRate;

            tubeScreamer.prepare(sampleRate);
            mxrDistiortion.prepare(sampleRate);
        }

		void process(T* x, const int& channel, const int& bufferSize)
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
            distortionSetting = distortionType;
        }

		int getType()
        {
            return distortionSetting;
        }

		void setMix(float mix)
        {
            this->mix = mix / 100;
        }
        float getMix()
        {
            return mix;
        }

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
