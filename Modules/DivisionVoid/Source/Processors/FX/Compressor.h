/*
  ==============================================================================

    Compressor.h
    Created: 12 Jan 2021 9:48:13pm
    Author:  Conor Foran

  ==============================================================================
*/

#pragma once

#define _USE_MATH_DEFINES

#include <cmath>
//#include <corecrt_math_defines.h>

#include "JuceHeader.h"

namespace DivisionVoid
{
    template<typename T>
    class Compressor
    {
    public:
        // Constructor
        Compressor() = default;

        // Destructor
        ~Compressor() = default;

        T processSample (T x, int channel)
        {
            // Initalise y
            float y = 0.0f;

            float xDetect = x;

            //xDetect = hpf->processSample(xDetect, channel);

            float x_Pos = fabs(xDetect); // fabs is abs for floats. Saves the conversion to double and back
            float x_dB = juce::Decibels::gainToDecibels(x_Pos);
            if (x_dB < -144.0f)
            {
                x_dB = -144.0f;
            }

            float xSC = 0;
            if (x_dB > (threshold + knee / 2.0f))
            {
                // Compression. Above the knee
                xSC = threshold + ((x_dB - threshold) / ratio);
            } else if (x_dB > (threshold - knee / 2.0f))
            {
                // Compression. Below the knee
                xSC = x_dB + ((1.0f / ratio - 1.0f) * powf(x_dB - threshold + knee / 2.0f, 2.0f)) / (2.0f * knee);
            } else
            {
                // No compression
                xSC = x_dB;
            }

            // Compare desired amplitude to in
            float gainChange = xSC - x_dB;

            if (progDependOn == true)
            {
                release = 1.0f / (-prevGainChange[channel] + 0.1f);
                alphaR = expf(-logf(9.0f) / (Fs * (release) * 0.001f));
            }

            // Gain smoothing
            float gainChangeSmooth;
            if (gainChange < prevGainChange[channel])
            {
                // Attack
                gainChangeSmooth = (1 - alphaA) * gainChange + alphaA * prevGainChange[channel];
            } else
            {
                // Release
                gainChangeSmooth = (1 - alphaR) * gainChange + alphaR * prevGainChange[channel];
            }

            gLin = powf(10.0f, gainChangeSmooth / 20.0f);

            //gainReductionValues.addValueToFifo(gLin);

            prevGainChange[channel] = gainChangeSmooth;

            y = gLin * x;

            return y;
        }

        void processChannel (T *x, int channel, const int &bufferSize)
        {
            for (auto i = 0; i < bufferSize; ++i)
            {
                T y = 0.0;

                T xDetect = x[i];

                if (hpfOn)
                    xDetect = hpf[channel].processSingleSampleRaw(xDetect);


                T x_Pos = fabs(xDetect);
                T x_dB = juce::Decibels::gainToDecibels(x_Pos);

                if (x_dB < -144.0f)
                {
                    x_dB = -144.0f;
                }

                T xSC = 0;
                if (knee < 2 * (x_dB - threshold))
                {
                    // Compression. Above the knee
                    xSC = threshold + ((x_dB - threshold) / ratio);
                } else if (x_dB > (threshold - knee / 2.0f))
                {
                    // Compression. Below the knee
                    xSC = x_dB + ((1.0f / ratio - 1.0f) * powf(x_dB - threshold + knee / 2.0f, 2.0f)) / (2.0f * knee);
                } else
                {
                    // No compression
                    xSC = x_dB;
                }

                // Compare desired amplitude to in
                T gainChange = xSC - x_dB;

                if (progDependOn == true)
                {
                    release = 1.0 / (-prevGainChange[channel] + 0.1);
                    alphaR = expf(-logf(9.0) / (Fs * (release) * 0.001));
                }

                // Gain smoothing
                T gainChangeSmooth;
                if (gainChange < prevGainChange[channel])
                {
                    // Attack
                    gainChangeSmooth = (1 - alphaA) * gainChange + alphaA * prevGainChange[channel];
                } else
                {
                    // Release
                    gainChangeSmooth = (1 - alphaR) * gainChange + alphaR * prevGainChange[channel];
                }

                gLin = powf(10.0f, gainChangeSmooth / 20.0f);

                //gainReductionValues.addValueToFifo(gLin);

                prevGainChange[channel] = gainChangeSmooth;

                y = gLin * x[i];

                if (harmonicContentOn)
                    y = harmonicContent(y);

                x[i] = y;
            }
        }

        /*void processStereo(float *xLeft, float *xRight, const int &bufferSize)
        {
            for (auto i = 0; i < bufferSize; ++i)
            {
                // Initalise y
                float yL = 0.0f;
                float yR = 0.0f;

                float xDetectLeft = xLeft[i];
                float xDetectRight = xRight[i];

                //xDetectLeft = hpf->processSample(xDetectLeft, 0);
                //xDetectRight = hpf->processSample(xDetectRight, 1);

                float xL_Pos = fabs(xDetectLeft); // fabs is abs for floats. Saves the conversion to double and back
                float xR_Pos = fabs(xDetectRight);

                float xL_dB = 20 * log10f(xL_Pos);
                float xR_dB = 20 * log10f(xR_Pos);

                if (xL_dB < -144.0f)
                {
                    xL_dB = -144.0f;
                }
                if (xR_dB < -144.0f)
                {
                    xR_dB = -144.0f;
                }

                float xLSC = 0.0f;
                float xRSC = 0.0f;

                if (xL_dB > (threshold + knee / 2.0f))
                {
                    // Compression. Above the knee
                    xLSC = threshold + ((xL_dB - threshold) / ratio);
                }
                else if (xL_dB > (threshold - knee / 2.0f))
                {
                    // Compression. Below the knee
                    xLSC = xL_dB + ((1.0f / ratio - 1.0f) * powf(xL_dB - threshold + knee / 2.0f, 2.0f)) / (2.0f * knee);
                }
                else
                {
                    // No compression
                    xLSC = xL_dB;
                }

                if (xR_dB > (threshold + knee / 2.0f))
                {
                    // Compression. Above the knee
                    xRSC = threshold + ((xR_dB - threshold) / ratio);
                }
                else if (xR_dB > (threshold - knee / 2.0f))
                {
                    // Compression. Below the knee
                    xRSC = xR_dB + ((1.0f / ratio - 1.0f) * powf(xR_dB - threshold + knee / 2.0f, 2.0f)) / (2.0f * knee);
                }
                else
                {
                    // No compression
                    xRSC = xR_dB;
                }

                // Compare desired amplitude to in
                float gainChangeL = xLSC - xL_dB;
                float gainChangeR = xRSC - xR_dB;

                if (progDependOn == true)
                {
                    release = 1.0f / (-prevGainChange[0] + 0.1f);
                    alphaR = expf(-logf(9.0f) / (Fs * (release) * 0.001f));
                }

                // Gain smoothing
                float gainChangeSmoothL;
                float gainChangeSmoothR;

                if (gainChangeL < prevGainChange[0])
                {
                    // Attack
                    gainChangeSmoothL = (1 - alphaA) * gainChangeL + alphaA * prevGainChange[0];
                }
                else
                {
                    // Release
                    gainChangeSmoothL = (1 - alphaR) * gainChangeL + alphaR * prevGainChange[0];
                }

                if (gainChangeR < prevGainChange[1])
                {
                    // Attack
                    gainChangeSmoothR = (1 - alphaA) * gainChangeR + alphaA * prevGainChange[1];
                }
                else
                {
                    // Release
                    gainChangeSmoothR = (1 - alphaR) * gainChangeR + alphaR * prevGainChange[1];
                }

                gLin = powf(10.0f, gainChangeSmoothL / 20.0f);

                prevGainChange[0] = gainChangeSmoothL;

                yL = gLin * xLeft[i];
                yL = harmonicContent(yL);

                xLeft[i] = yL;

                gLin = powf(10.0f, gainChangeSmoothR / 20.0f);

                prevGainChange[1] = gainChangeSmoothR;

                yR = gLin * xRight[i];
                yR = harmonicContent(yR);

                xRight[i] = yR;
            }
        }*/

        T getGainReduction ()
        {
            return gLin;
        }

        void setThreshold (T threshold)
        {
            if (threshold <= 0.0f && threshold >= -64.0f)
                this->threshold = threshold;
        }
        T getThreshold ()
        {
            return threshold;
        }

        void setRatio (T ratio)
        {
            if (ratio <= 10.0f && ratio >= 1.0f)
                this->ratio = ratio;
        }
        T getRatio ()
        {
            return ratio;
        }

        void setKnee (T knee)
        {
            if (knee <= 10.0f && knee >= 0.0f)
                this->knee = knee;
        }
        T getKnee ()
        {
            return knee;
        }

        void setAttack (T attackMs)
        {
            attack = attackMs / 1000;
            if (attack <= 0.5f && attack >= 0.001f)
            {
                this->attack = attack;
                alphaA = expf(-logf(9.0f) / (Fs * attack));
            }
        }
        T getAttack ()
        {
            return attack;
        }

        void setRelease (T releaseMs)
        {
            release = releaseMs / 1000;
            if (release <= 1.0f && release >= 0.01f)
            {
                this->release = release;
                alphaR = expf(-logf(9.0f) / (Fs * release));
            }
        }
        T getRelease ()
        {
            return release;
        }

        void setHPFFreq (T hpfFreq)
        {
            if (hpfFreq <= 500.0f && hpfFreq >= 0.1f)
            {
                this->hpfFreq = hpfFreq;
                hpf[0].setCoefficients(juce::IIRCoefficients::makeHighPass(Fs, hpfFreq, 0.7));
                hpf[1].setCoefficients(juce::IIRCoefficients::makeHighPass(Fs, hpfFreq, 0.7));
            }
        }
        T getHPFFreq ()
        {
            return hpfFreq;
        }

        void engageHPF (bool isEngaged)
        {
            hpfOn = isEngaged;
        }

        void engageHarmonicContent (bool isEngaged)
        {
            harmonicContentOn = isEngaged;
        }

        void setSampleRate (T Fs)
        {
            this->Fs = Fs;
            hpf[0].setCoefficients(juce::IIRCoefficients::makeHighPass(Fs, hpfFreq, 0.7));
            hpf[1].setCoefficients(juce::IIRCoefficients::makeHighPass(Fs, hpfFreq, 0.7));
        }
        T getSampleRate ()
        {
            return Fs;
        }

        void setProgDependOn (bool progDependOn)
        {
            this->progDependOn = progDependOn;
        }
        bool getProgDependOn ()
        {
            return progDependOn;
        }

        T harmonicContent (T input)
        {
            return input - (drive * (1 / 2) * powf(input, 2)) - (drive * (1 / 3) * powf(input, 3));
        }

        void setDrive (T drive)
        {
            if (drive >= 0.0f && drive <= 1.0f)
                this->drive = drive;
        }

        //const float* getGainReductionValues();

    private:
        T Fs = 48000;

        T threshold = 0.0; // dB
        T ratio = 1.0;
        T knee = 0.0; // dB
        T attack = 0.1; // seconds
        T release = 0.1;
        T hpfFreq = 0.1; // Hz

        bool progDependOn = false;
        bool hpfOn = false;
        bool harmonicContentOn = false;

        T drive = 0.8;

        T alphaA = 0.9995;    // Smoothing filter coefficients
        T alphaR = 0.9995;

        T prevGainChange[2] = {0.0, 0.0};

        juce::IIRFilter hpf[2];

        T gLin;
    };
}


