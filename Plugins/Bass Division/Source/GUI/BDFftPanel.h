/*
  ==============================================================================

    BDFftPanel.h
    Created: 21 Feb 2021 3:59:03pm
    Author:  Olorin

  ==============================================================================
*/

#pragma once

#include "BDPanelBase.h"
#include "FFT/SpectrumViewer.h"

enum FFTOrder
{
    order2048 = 11,
    order4096 = 12,
    order8192 = 13
};

template<typename BlockType>
struct FFTDataGenerator
{
    void produceFFTDataForRendering(const juce::AudioBuffer<float>& audioData, const float negInfinity)
    {
        const auto fftSize = getFFTSize();
        
        fftData.assign(fftData.size(), 0);
        auto* readIndex = audioData.getReadPointer(0);
        std::copy(readIndex, readIndex + fftSize, fftData.begin());
        
        window->multiplyWithWindowingTable(fftData.data(), fftSize);
        
        forwardFFT->performFrequencyOnlyForwardTransform(fftData.data());
        
        int numBins = (int)fftSize / 2;
        
        for (int i = 0; i < numBins; ++i)
        {
            fftData[i] /= (float)numBins;
        }
        
        for (int i = 0; i < numBins; ++i)
        {
            fftData[i] = juce::Decibels::gainToDecibels(fftData[i], negInfinity);
        }
        
        fftDataFifo.push(fftData);
    }
    
    void changeOrder(FFTOrder newOrder)
    {
        order = newOrder;
        auto fftSize = getFFTSize();
        
        forwardFFT = std::make_unique<juce::dsp::FFT>(order);
        window = std::make_unique<juce::dsp::WindowingFunction<float>>(fftSize, juce::dsp::WindowingFunction<float>::blackmanHarris);
        
        fftData.clear();
        fftData.resize(fftSize * 2, 0);
        fftDataFifo.prepare(fftData.size());
    }
    
    int getFFTSize() const { return 1 << order; }
    int getNumAvailableFFTDataBlocks() const { return fftDataFifo.getNumAvailableForReading(); }
    
    bool getFFTData(BlockType& fftData) { return fftDataFifo.pull(fftData); }
private:
    FFTOrder order;
    BlockType fftData;
    std::unique_ptr<juce::dsp::FFT> forwardFFT;
    std::unique_ptr<juce::dsp::WindowingFunction<float>> window;
    
    Fifo<BlockType> fftDataFifo;
};

template<typename PathType>
struct AnalyserPathGenerator
{
    void generatePath(const std::vector<float>& renderData, juce::Rectangle<float> fftBounds, int fftSize, float binWidth, float negInfinity)
    {
        auto top = fftBounds.getY();
        auto bottom = fftBounds.getHeight();
        auto width = fftBounds.getWidth();
        
        int numBins = (int)fftSize / 2;
        
        PathType p;
        p.preallocateSpace(3 * (int)fftBounds.getWidth());
        
        auto map = [bottom, top, negInfinity](float v)
        {
            return juce::jmap(v, negInfinity, 0.0f, (float)bottom, top);
        };
        
        auto y = map(renderData[0]);
        
        jassert(!std::isnan(y) && !std::isinf(y));
        
        p.startNewSubPath(0, y);
        
        const int pathResolution = 1;
        
        for (int binNum = 1; binNum < numBins; binNum += pathResolution)
        {
            y = map(renderData[binNum]);
            
            jassert(!std::isnan(y) && !std::isinf(y));
            
            if (!std::isnan(y) && !std::isinf(y))
            {
                auto binFreq = binNum * binWidth;
                auto normalisedBinX = juce::mapFromLog10(binFreq, 20.0f, 20000.0f);
                int binX = std::floor(normalisedBinX * width);
                p.lineTo(binX, y);
            }
        }
        
        pathFifo.push(p);
    }
    
    int getNumPathsAvailable() const
    {
        return pathFifo.getNumAvailableForReading();
    }
    
    bool getPath(PathType& path)
    {
        return pathFifo.pull(path);
    }
private:
    Fifo<PathType> pathFifo;
};

struct EqCurve : public BDPanelBase, public juce::Timer, public juce::AudioProcessorParameter::Listener
{
public:
    EqCurve(BassDivisionAudioProcessor& inProcessor);
    ~EqCurve();
    
    void paint(juce::Graphics& g) override;
    void resized() override;
    
    void timerCallback() override;
    
    void parameterValueChanged(int parameterIndex, float newValue) override;
    void parameterGestureChanged(int parameterIndex, bool gestureIsStarting) override {}
    
private:
    MonoChain monochain;
    juce::Atomic<bool> parametersChanged {false};
    
    void updateMonoChain();
    
    juce::Rectangle<int> getRenderArea();
    juce::Rectangle<int> getAnalysisArea();
    
    juce::Image background;
    
    BassDivisionAudioProcessor& audioProcessor;
    
    SingleChannelSampleFifo<BassDivisionAudioProcessor::BlockType>* leftChannelFifo;
    
    juce::AudioBuffer<float> monoBuffer;
    FFTDataGenerator<std::vector<float>> leftChannelFFTDataGenerator;
    AnalyserPathGenerator<juce::Path> pathProducer;
    
    juce::Path leftChannelFFTPath;
};

class BDFftPanel : public BDPanelBase
{
public:
    BDFftPanel (BassDivisionAudioProcessor& inProcessor,
                juce::Value& repaintSpectrumViewer,
                drow::Buffer& spectrumMagnitudeBuffer,
                juce::Value& detectedFrequency);
    ~BDFftPanel();

	void paint(juce::Graphics& g) override;
	void resized() override;

private:
    //EqCurve eqCurve;
    // Values needed for the spectrum viewer, plus the comopnent itself
    SpectrumViewer spectrumViewer;
    juce::Value sampleRate;
    juce::Label header;
    
    BassDivisionAudioProcessor& audioProcessor;
};
