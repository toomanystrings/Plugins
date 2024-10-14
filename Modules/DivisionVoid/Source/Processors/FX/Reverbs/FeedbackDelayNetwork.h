//
// Created by Conor Foran on 12/10/2024.
//

#pragma once

#include "../Filters/FractionalDelay.h"

namespace DivisionVoid
{
    template<typename T>
    class FeedbackDelayNetwork
    {
    public:
        FeedbackDelayNetwork();

        ~FeedbackDelayNetwork() = default;

        T processSample(T x, int channel);

        void setSampleRate(float sampleRate);

        void setTime(float timeValue);

        void setDepth(float modValue);

    private:
        DivisionVoid::FractionalDelay<T> fractionalDelay[4];
        float delayLength[4] = {1789.0f, 2281.0f, 2593.0f, 3463.0f};

        float Fs = 48000.0f;

        float g1[4] = {0.0f, 1.0f, 1.0f, 0.0f};
        float g2[4] = {-1.0f, 0.0f, 0.0f, -1.0f};
        float g3[4] = {1.0f, 0.0f, 0.0f, -1.0f};
        float g4[4] = {0.0f, 1.0f, -1.0f, 0.0f};

        float feedbackGain = 0.5f;

        T fb1[2] = {0.0f};
        T fb2[2] = {0.0f};
        T fb3[2] = {0.0f};
        T fb4[2] = {0.0f};
    };
}
