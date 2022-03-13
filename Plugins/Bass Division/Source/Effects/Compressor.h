/*
  ==============================================================================

    Compressor.h
    Created: 12 Jan 2021 9:48:13pm
    Author:  Olorin

  ==============================================================================
*/

#pragma once

#define _USE_MATH_DEFINES

#include <cmath>
//#include <corecrt_math_defines.h>

#include <JuceHeader.h>
#include "../FIFO.h"

//#include 

class Compressor
{
public:
	// Constructor
	Compressor() : gainReductionValues(512, false)
	{
		//hpf = new Biquad();
		//hpf->setFs(48000);
		//hpf->setFreq(1.0f);
		//hpf->setQ(0.707f);
		//hpf->filterType = Biquad::HPF;
	};

	// Destructor
	~Compressor() { /*delete hpf; */};

	float processSample(float x, int channel);
	void processChannel(float* x, int channel, const int& bufferSize);
	void processStereo(float* xLeft, float* xRight, const int& bufferSize);

	float getGainReduction();

	void setThreshold(float threshold);
	float getThreshold();

	void setRatio(float ratio);
	float getRatio();

	void setKnee(float knee);
	float getKnee();

	void setAttack(float attack);
	float getAttack();

	void setRelease(float release);
	float getRelease();

	void setHPFFreq(float hpfFreq);
	float getHPFFreq();

	void engageHPF(bool isEngaged);
	void engageHarmonicContent(bool isEngaged);

	void setSampleRate(float Fs);
	float getSampleRate();

	void setProgDependOn(bool progDependOn);
	bool getProgDependOn();

	float harmonicContent(float input);
	void setDrive(float drive);
	

	const float* getGainReductionValues();

private:
	float Fs = 48000;

	float threshold = 0.0f; // dB
	float ratio = 1.0f;
	float knee = 0.0f; // dB
	float attack = 0.1f; // seconds
	float release = 0.1f;
	float hpfFreq = 0.1f; // Hz

	bool progDependOn = false;
	bool hpfOn = false;
	bool harmonicContentOn = false;

	float drive = 0.8f;

	float alphaA = 0.9995f;	// Smoothing filter coefficients
	float alphaR = 0.9995f;

	float prevGainChange[2] = { 0.0f };

	//Biquad* hpf;

	juce::IIRFilter hpf[2];

	float gLin;

	DivisionVoid::FIFO gainReductionValues;
};
