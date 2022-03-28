/*
  ==============================================================================

    MXRClipper.cpp
    Created: 26 Mar 2021 11:24:30pm
    Author:  Olorin

  ==============================================================================
*/

#include "MXRClipper.h"

template <typename T>
float MXRClipper<T>::processSample(T sample, const int& channel)
{
	// First, apply the gain stage
    float Vy = b0 * sample - x3[channel];
    x3[channel] = (2 / R3) * sample - x3[channel];

	// Damped Newton-Raphson method for solving the clipper
    int iter = 1;
    float b = 1.0f;
    float fd = -Vy / R2 + 2 * Is * sinh(Vd[channel] / (eta * Vt)) + (1 / R1 + 1 / R2) * Vd[channel] - x1[channel];
	
    while (iter < 50 && abs(fd) > thr) 
    {
        float den = 2 * Is / (eta * Vt) * cosh(Vd[channel] / (eta * Vt)) + (1 / R1 + 1 / R2);
        float Vnew = Vd[channel] - b * fd / den;
        float fn = -Vy / R2 + 2 * Is * sinh(Vnew / (eta * Vt)) + (1 / R1 + 1 / R2) * Vnew - x1[channel];
    	
        if (abs(fn) < abs(fd)) 
        {
            Vd[channel] = Vnew;
            b = 1.0f;
        }
        else 
        {
            b *= 0.5f;
        }
    	
        fd = -Vy / R2 + 2 * Is * sinh(Vd[channel] / (eta * Vt)) + (1 / R1 + 1 / R2) * Vd[channel] - x1[channel];
        iter++;
    }

	float Vo = Vd[channel] * G;
    x1[channel] = (2.0f / R1) * Vd[channel] - x1[channel];

    return Vo;
}

template <typename T>
void MXRClipper<T>::processBuffer(T* Vi, const int& channel, const int& bufferSize)
{
    for (int i = 0; i < bufferSize; ++i)
    {
        // First, apply the gain stage
        float Vy = b0 * Vi[i] - x3[channel];
        x3[channel] = (2 / R3) * Vi[i] - x3[channel];

        // Damped Newton-Raphson method for solving the clipper
        int iter = 1;
        float b = 1.0f;
        float fd = -Vy / R2 + 2 * Is * sinh(Vd[channel] / (eta * Vt)) + (1 / R1 + 1 / R2) * Vd[channel] - x1[channel];

        while (iter < 50 && abs(fd) > thr)
        {
            float den = 2 * Is / (eta * Vt) * cosh(Vd[channel] / (eta * Vt)) + (1 / R1 + 1 / R2);
            float Vnew = Vd[channel] - b * fd / den;
            float fn = -Vy / R2 + 2 * Is * sinh(Vnew / (eta * Vt)) + (1 / R1 + 1 / R2) * Vnew - x1[channel];

            if (abs(fn) < abs(fd))
            {
                Vd[channel] = Vnew;
                b = 1.0f;
            }
            else
            {
                b *= 0.5f;
            }

            fd = -Vy / R2 + 2 * Is * sinh(Vd[channel] / (eta * Vt)) + (1 / R1 + 1 / R2) * Vd[channel] - x1[channel];
            iter++;
        }

        float Vo = Vd[channel] * G;
        x1[channel] = (2.0f / R1) * Vd[channel] - x1[channel];

        Vi[i] = Vo;
    }
}

template <typename T>
void MXRClipper<T>::prepare(double sampleRate)
{
    this->sampleRate = sampleRate;
    Ts = 1.0f / sampleRate;
    updateCoefficients();
}

template <typename T>
void MXRClipper<T>::setDrive(float drive)
{
    if (drive > 0.95)
        this->drive = drive;
    else
        this->drive = 0.95f;
    
    b0 = 1 + (R8 / (((1 - this->drive) * Rdist) + R7 + R3));
    
    
}

template <typename T>
void MXRClipper<T>::setOutput(float output)
{
    this->output = output;
    G = (output * Rout) / (((1 - output) * Rout) + (output * Rout));
}

template <typename T>
void MXRClipper<T>::updateCoefficients()
{
    float R1 = Ts / (2.0f * C1);
    float R3 = Ts / (2.0f * C3);
}
