/*
  ==============================================================================

    MXRClipper.h
    Created: 26 Mar 2021 11:24:30pm
    Author:  Olorin

  ==============================================================================
*/
#include <JuceHeader.h>

#pragma once
class MXRClipper
{
public:
    MXRClipper(){}
	~MXRClipper(){}

    float processSample(float sample, const int& channel);
    void processBuffer(float* Vi, const int& channel, const int& bufferSize);

    void prepare(double sampleRate);

    void setDrive(float drive);
    void setOutput(float output);

private:
    void updateCoefficients();

    double sampleRate = 44100;
    float Ts = 1 / sampleRate;

    float drive = 0.5f;
    float output = 0.5f;
	
    // Diode values - set for germanium
    float eta = 1.5f;
    float Is = 1e-6f;
    float Vt = 26e-3f;

	// Component Values
    float C1 = 1e-9f;
    float C3 = 47e-9f;

	float R1 = Ts / (2.0f * C1);
    float R2 = 10e3f;
	float R3 = Ts / (2.0f * C3);
    float R7 = 4.7e3f;
    float R8 = 1e6;

    float Rdist = 1e6f;
    float Rout = 1e6;

    float b0 = 1 + (R8 / (((1 - drive) * Rdist) + R7 + R3));
    float G = (output * Rout) / (((1 - output) * Rout) + (output * Rout));

    float thr = 0.00000000001f;

    float x1[2] = { 0.0f };
    float x3[2] = { 0.0f };
    float Vd[2] = { 0.0f };
};
