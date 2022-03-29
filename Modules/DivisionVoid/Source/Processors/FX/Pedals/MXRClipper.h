/*
  ==============================================================================

    MXRClipper.h
    Created: 26 Mar 2021 11:24:30pm
    Author:  Olorin

  ==============================================================================
*/
#include "JuceHeader.h"

#pragma once

template <typename T>
class MXRClipper
{
public:
    MXRClipper(){}
	~MXRClipper(){}

    float processSample(T sample, const int& channel)
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

    void processBuffer(T* Vi, const int& channel, const int& bufferSize)
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

    void prepare(double sampleRate)
    {
        this->sampleRate = sampleRate;
        Ts = 1.0f / sampleRate;
        updateCoefficients();
    }

    void setDrive(float drive)
    {
        if (drive > 0.95)
            this->drive = drive;
        else
            this->drive = 0.95f;

        b0 = 1 + (R8 / (((1 - this->drive) * Rdist) + R7 + R3));
    }

    void setOutput(float output)
    {
        this->output = output;
        G = (output * Rout) / (((1 - output) * Rout) + (output * Rout));
    }

private:
    void updateCoefficients()
    {
        float R1 = Ts / (2.0f * C1);
        float R3 = Ts / (2.0f * C3);
    }

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
