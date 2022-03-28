/*
  ==============================================================================

    TSClipper.h
    Created: 7 Feb 2021 5:44:27pm
    Author:  Olorin

  ==============================================================================
*/

#pragma once
template <typename T>
class TSClipper
{
public:
    TSClipper(){};
	~TSClipper(){};

	// Callback Functions
	float processSample(T Vi, int channel);

    void processBuffer(T* Vi, const int& channel, const int& bufferSize);

	void prepare(float sampleRate);

    void prepare(float sampleRate, int factor);

	void setKnob(float drive);

	

private:
    const float eta = 1.0f; // Change for non-ideal diode
    const float Is = 1e-15;
    const float Vt = 26e-3;

    float sampleRate = 48000.0f;
    float Ts = 1.0f / sampleRate;

    float C1 = 47e-9;
    float R1 = Ts / (2.0f * C1);
    float C2 = 51e-12;
    float R2 = Ts / (2.0 * C2);
    float drivePot = 1.0f;
    float P1 = drivePot * 500e3;
    float R3 = 51000.0f + P1;
    float R4 = 4700.0f;

    // Combined Resistances
    float G1 = (1.0f + R4 / R1);
    float G4 = (1.0f + R1 / R4);

    // States
    float x1[2] = { 0.0f };
    float x2[2] = { 0.0f };
    float Vd[2] = { 0.0f };

    float thr = 0.00000000001f;
	
	void updateCoefficients();


	
};


