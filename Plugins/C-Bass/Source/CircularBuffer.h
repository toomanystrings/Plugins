//
// Created by Conor Foran on 05/06/2026.
//

#pragma once

#include "JuceHeader.h"
#include <vector>

// =============================================================================
// CircularBuffer
// =============================================================================
// Fixed-size ring buffer. Write advances a pointer; read looks N positions back.
// readDelayedInterpolated() uses linear interpolation to prevent zipper noise
// when the delay position is modulated.
// =============================================================================

struct CircularBuffer
{
public:
    void prepare(int sizeInSamples);
    void write(float sample);

    float readDelayed(int delaySamples) const;
    float readDelayedInterpolated(float delaySamples) const;

    float getAverageAmplitude(int nSamples) const;

    int getSize() const;

private:
    std::vector<float> buffer;
    int writeIndex = 0;
    int bufferSize = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CircularBuffer)
};
