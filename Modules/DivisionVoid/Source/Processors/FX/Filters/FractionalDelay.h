//
// Created by Conor Foran on 07/10/2024.
//

#pragma once

#include <cmath>

namespace DivisionVoid
{
    template<typename T>
    class FractionalDelay
    {
    public:
        FractionalDelay() = default;

        FractionalDelay(float delaySamples, float speed) : delay (delaySamples), speed (speed) {}

        ~FractionalDelay() = default;

        T processSample(T x, int channel)
        {
            if (delay < 1.0f)
                return x;

            double twoPi = M_PI * 2.0f;
            float lfo = depth * std::sin(currentAngle[channel]);

            currentAngle[channel] += angleChange;
            if (currentAngle[channel] > twoPi)
                currentAngle[channel] -= twoPi;

            // Delay buffer
            // delay can be a fraction
            int d1 = static_cast<int>(std::floor(delay + lfo));
            int d2 = d1 + 1;
            float g2 = delay + lfo - static_cast<float>(d1);
            float g1 = 1.0f - g2;

            int indexD1 = index[channel] - d1;
            if (indexD1 < 0)
                indexD1 += MAX_BUFFER_SIZE;

            int indexD2 = index[channel] - d2;
            if (indexD2 < 0)
                indexD2 += MAX_BUFFER_SIZE;

            float y = g1 * delayBuffer[indexD1][channel] + g2 * delayBuffer[indexD2][channel];

            delayBuffer[index[channel]][channel] = x;

            if (index[channel] < MAX_BUFFER_SIZE - 1)
                index[channel]++;
            else
                index[channel] = 0;

            return y;
        }

        void setSampleRate(float Fs)
        {
            this->Fs = Fs;
            updateAngleChange();
        }

        void setDelaySamples(float delay)
        {
            if (delay >= 1.0f)
                this->delay = delay;
            else
                this->delay = 0.0f;
        }

        void setSpeed(float speed)
        {
            if (speed >= 0.1f && speed <= 10.0f)
            {
                this->speed = speed;
                updateAngleChange();
            }
        }

        void setDepth(float depth) { this->depth = depth; }
    private:

        void updateAngleChange() { this->angleChange = speed * (1.0f / Fs) * 2.0f * M_PI; }

        float Fs = 48000.0f;
        float delay = 5.0f;

        const int MAX_BUFFER_SIZE = 96000;
        float delayBuffer[96000][2] = {0.0f};
        int index[2] = {0};

        float speed = 1.0f;
        float depth = 10.0f;

        float currentAngle[2] = {0.0f};
        float angleChange = speed * (1.0f / Fs) * 2.0f * M_PI;
    };
}