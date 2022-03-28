/*
  ==============================================================================

    TSClipper.cpp
    Created: 7 Feb 2021 5:44:27pm
    Author:  Olorin

  ==============================================================================
*/

#include "TSClipper.h"
#include "JuceHeader.h"

template <typename T>
float TSClipper<T>::processSample(T Vi, int channel)
{
    float p = -Vi / (G4 * R4) + R1 / (G4 * R4) * x1[channel] - x2[channel];

    int iter = 1;
    float b = 1.0f;
    float fd = p + Vd[channel] / R2 + Vd[channel] / R3 + 2.f * Is * sinh(Vd[channel] / (eta * Vt));
    while (iter < 50 && abs(fd) > thr) {
        float den = 2.f * Is / (eta * Vt) * cosh(Vd[channel] / (eta * Vt)) + 1.f / R2 + 1.f / R3;
        float Vnew = Vd[channel] - b * fd / den;
        float fn = p + Vnew / R2 + Vnew / R3 + 2.f * Is * sinh(Vnew / (eta * Vt));
        if (abs(fn) < abs(fd)) {
            Vd[channel] = Vnew;
            b = 1.0f;
        }
        else {
            b *= 0.5f;
        }
        fd = p + Vd[channel] / R2 + Vd[channel] / R3 + 2.0f * Is * sinh(Vd[channel] / (eta * Vt));
        iter++;
    }
    float Vo = Vd[channel] + Vi;
    x1[channel] = (2.0f / R1) * (Vi / G1 + x1[channel] * R4 / G1) - x1[channel];
    x2[channel] = (2.0f / R2) * (Vd[channel]) - x2[channel];

    return Vo * 2 / 3;
}

template <typename T>
void TSClipper<T>::processBuffer(T* Vi, const int& channel, const int& bufferSize)
{
	for (int i = 0; i < bufferSize; ++i)
	{
        float p = -Vi[i] / (G4 * R4) + R1 / (G4 * R4) * x1[channel] - x2[channel];

        int iter = 1;
        float b = 1.0f;
        float fd = p + Vd[channel] / R2 + Vd[channel] / R3 + 2.f * Is * sinh(Vd[channel] / (eta * Vt));
        while (iter < 50 && abs(fd) > thr) {
            float den = 2.f * Is / (eta * Vt) * cosh(Vd[channel] / (eta * Vt)) + 1.f / R2 + 1.f / R3;
            float Vnew = Vd[channel] - b * fd / den;
            float fn = p + Vnew / R2 + Vnew / R3 + 2.f * Is * sinh(Vnew / (eta * Vt));
            if (abs(fn) < abs(fd)) {
                Vd[channel] = Vnew;
                b = 1.0f;
            }
            else {
                b *= 0.5f;
            }
            fd = p + Vd[channel] / R2 + Vd[channel] / R3 + 2.0f * Is * sinh(Vd[channel] / (eta * Vt));
            iter++;
        }
        float Vo = Vd[channel] + Vi[i];
        x1[channel] = (2.0f / R1) * (Vi[i] / G1 + x1[channel] * R4 / G1) - x1[channel];
        x2[channel] = (2.0f / R2) * (Vd[channel]) - x2[channel];

        Vi[i] =  Vo * 2 / 3;
	}
}

template <typename T>
void TSClipper<T>::prepare(float sampleRate)
{
	if (this->sampleRate != sampleRate)
	{
		this->sampleRate = sampleRate;
		updateCoefficients();
	}
}

template <typename T>
void TSClipper<T>::prepare(float sampleRate, int factor)
{
    if (this->sampleRate != sampleRate * factor)
    {
        this->sampleRate = sampleRate * factor;
        updateCoefficients();
    }
}

template <typename T>
void TSClipper<T>::setKnob(float drive)
{
	if (this->drivePot != drive)
	{
		this->drivePot = drive;
		updateCoefficients();
	}
}

template <typename T>
void TSClipper<T>::updateCoefficients()
{
	
    Ts = 1.0f / sampleRate;
    R1 = Ts / (2.0f * C1);
    R2 = Ts / (2.0f * C2);
    P1 = drivePot * 500e3;
    R3 = 51000.0f + P1;
    R4 = 4700.0f;

    // Combined Resistances
    G1 = (1.0f + R4 / R1);
    G4 = (1.0f + R1 / R4);
}
