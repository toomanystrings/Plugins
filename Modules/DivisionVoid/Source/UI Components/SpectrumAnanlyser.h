#pragma once

namespace DivisionVoid
{
class SpectrumAnalyserComponent : public juce::Component,
                         private juce::Timer
{
public:
    enum
    {
        fftOrder = 11,
        fftSize = 1 << fftOrder,
        scopeSize = 512
    };

    SpectrumAnalyserComponent () : forwardFFT(fftOrder),
                                   window(fftSize, juce::dsp::WindowingFunction<float>::hann)
    {

    }
    ~SpectrumAnalyserComponent () = default;

    void paint (juce::Graphics& g) override
    {

    }

    void resized () override
    {

    }

    void drawNextFrameOfSpectrum()
    {
        // first apply a windowing function to our data
        window.multiplyWithWindowingTable (fftData, fftSize);
        // then render our FFT data..
        forwardFFT.performFrequencyOnlyForwardTransform (fftData);
        auto mindB = -100.0f;
        auto maxdB = 0.0f;
        for (int i = 0; i < scopeSize; ++i)
        {
            auto skewedProportionX = 1.0f - std::exp (std::log (1.0f - (float) i / (float) scopeSize) * 0.2f);
            auto fftDataIndex = juce::jlimit (0, fftSize / 2, (int) (skewedProportionX * (float) fftSize * 0.5f));
            auto level = juce::jmap (juce::jlimit (mindB, maxdB, juce::Decibels::gainToDecibels (fftData[fftDataIndex]) - juce::Decibels::gainToDecibels ((float) fftSize)),
                                     mindB,
                                     maxdB,
                                     0.0f,
                                     1.0f);
            scopeData[i] = level;
        }
    }

    void retrieveFFTData(float* samples)
    {

    }

    bool setDataProcessed()
    {

    }



private:

    void timerCallback() override
    {

    }

    juce::dsp::FFT forwardFFT;
    juce::dsp::WindowingFunction<float> window;

    //float fifo[fftSize];
    float fftData[2 * fftSize];
    int fifoIndex = 0;
    bool nextFFTBlockReady = false;
    float scopeData[scopeSize];

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SpectrumAnalyserComponent)
};
}
