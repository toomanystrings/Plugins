//
// Created by Conor Foran on 12/09/2024.
//

#pragma once

namespace DivisionVoid
{
    class AllPassFilter
    {
    public:
        AllPassFilter() { reset(); }
        AllPassFilter(float coefficient) : prevX(0.0f), prevY(0.0f)
        {
            jassert(coefficient < 1.0f && coefficient > -1.0f);
            this -> a = coefficient;
            reset();
        }

        ~AllPassFilter() = default;

        float processSample(float x)
        {
            // y[n]=a⋅x[n]+x[n−1]−a⋅y[n−1]
            float y = a * x + prevX - a * prevY;

            prevX = x;
            prevY = y;

            return y;
        }

        void process(float* buffer, int bufferSize)
        {
            for (int i = 0; i < bufferSize; ++i)
            {
                buffer[i] = processSample(buffer[i]);
            }
        }

        void setFilterCoefficient(float newCoefficient)
        {
            jassert(newCoefficient < 1.0f && newCoefficient > -1.0f);
            a = newCoefficient;
        }
        float getFilterCoefficient() { return a; }

        void reset()
        {
            prevX = 0.0f;
            prevY = 0.0f;
        }

    private:
        float prevX, prevY = 0.0f;

        // Filter coefficient
        float a = 0.5f;
    };
}
