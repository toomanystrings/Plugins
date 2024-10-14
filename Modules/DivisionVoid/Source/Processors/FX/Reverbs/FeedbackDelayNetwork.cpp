//
// Created by Conor Foran on 13/10/2024.
//

#include "FeedbackDelayNetwork.h"

template<typename T>
DivisionVoid::FeedbackDelayNetwork<T>::FeedbackDelayNetwork()
{
    for (int i = 0; i < sizeof (fractionalDelay); ++i)
        fractionalDelay[i].setDelaySamples(delayLength[i]);
}

template<typename T>
T DivisionVoid::FeedbackDelayNetwork<T>::processSample(T x, int channel)
{
    T y;

    T inDL1 = x + fb1[channel];
    T inDL2 = x + fb2[channel];
    T inDL3 = x + fb3[channel];
    T inDL4 = x + fb4[channel];

    T outDL1 = fractionalDelay[0].processSample(inDL1, channel);
    T outDL2 = fractionalDelay[1].processSample(inDL2, channel);
    T outDL3 = fractionalDelay[2].processSample(inDL3, channel);
    T outDL4 = fractionalDelay[3].processSample(inDL4, channel);

    y = 0.25f * (outDL1 + outDL2 + outDL3 + outDL4);

    fb1[channel] = ((outDL1 * g1[0]) + (outDL2 * g2[0]) + (outDL3 * g3[0]) + (outDL4 * g4[0])) * feedbackGain;
    fb2[channel] = ((outDL1 * g1[1]) + (outDL2 * g2[1]) + (outDL3 * g3[1]) + (outDL4 * g4[1])) * feedbackGain;
    fb3[channel] = ((outDL1 * g1[2]) + (outDL2 * g2[2]) + (outDL3 * g3[2]) + (outDL4 * g4[2])) * feedbackGain;
    fb4[channel] = ((outDL1 * g1[3]) + (outDL2 * g2[3]) + (outDL3 * g3[3]) + (outDL4 * g4[3])) * feedbackGain;

    return y;
}

template<typename T>
void DivisionVoid::FeedbackDelayNetwork<T>::setSampleRate(float sampleRate)
{
    this->Fs = sampleRate;
    for (int i = 0; i < sizeof (fractionalDelay); ++i)
        fractionalDelay[i].setSampleRate (sampleRate);
}

template<typename T>
void DivisionVoid::FeedbackDelayNetwork<T>::setTime(float timeValue)
{
    this->feedbackGain = timeValue;
}

template<typename T>
void DivisionVoid::FeedbackDelayNetwork<T>::setDepth(float modValue)
{
    for (int i = 0; i < sizeof (fractionalDelay); ++i)
        fractionalDelay[0].setDepth(modValue);
}



