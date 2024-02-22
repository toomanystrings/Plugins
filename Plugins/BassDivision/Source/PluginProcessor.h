/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "Managers/PresetManager.h"
#include "Managers/IrManager.h"

#include "FFT/SpectrumProcessor.h"

#include <array>
//template<typename T>
//struct Fifo
//{
//    void prepare(int numChannels, int numSamples)
//    {
//        for (auto& buffer : buffers)
//        {
//            buffer.setSize(numChannels, numSamples, false, true, true);
//            buffer.clear();
//        }
//    }
//
//    void prepare(size_t numElements)
//    {
//        for (auto& buffer : buffers)
//        {
//            buffer.clear();
//            buffer.resize(numElements, 0);
//        }
//    }
//
//    bool push(const T& t)
//    {
//        auto write = fifo.write(1);
//        if (write.blockSize1 > 0)
//        {
//            buffers[write.startIndex1] = t;
//            return true;
//        }
//
//        return false;
//    }
//
//    bool pull(T& t)
//    {
//        auto read = fifo.read(1);
//        if (read.blockSize1 > 0)
//        {
//            t = buffers[read.startIndex1];
//            return true;
//        }
//
//        return false;
//    }
//
//    int getNumAvailableForReading() const
//    {
//        return fifo.getNumReady();
//    }
//
//private:
//    static constexpr int Capacity = 30;
//    std::array<T, Capacity> buffers;
//    juce::AbstractFifo fifo {Capacity};
//};
//
//enum Channel
//{
//    Left,
//    Right
//};
//
//template<typename BlockType>
//struct SingleChannelSampleFifo
//{
//    SingleChannelSampleFifo(Channel ch) : channelToUse(ch)
//    {
//        prepared.set(false);
//    }
//
//    void update(const BlockType& buffer)
//    {
//        jassert(prepared.get());
//        jassert(buffer.getNumChannels() > channelToUse);
//        auto* channelPtr = buffer.getReadPointer(channelToUse);
//
//        for (int i = 0; i < buffer.getNumSamples(); ++i)
//        {
//            pushNextSampleIntoFifo(channelPtr[i]);
//        }
//    }
//
//    void prepare(int bufferSize)
//    {
//        prepared.set(false);
//        size.set(bufferSize);
//
//        bufferToFill.setSize(1, bufferSize, false, true, true);
//        audioBufferFifo.prepare(1, bufferSize);
//        fifoIndex = 0;
//        prepared.set(true);
//    }
//
//    int getNumCompleteBuffersAvailable() const { return audioBufferFifo.getNumAvailableForReading(); }
//    bool isPrepared() const { return prepared.get(); }
//    int getSize() const { return size.get(); }
//
//    bool getAudioBuffer(BlockType& buf) { return audioBufferFifo.pull(buf); }
//
//private:
//    Channel channelToUse;
//    int fifoIndex = 0;
//    Fifo<BlockType> audioBufferFifo;
//    BlockType bufferToFill;
//    juce::Atomic<bool> prepared = false;
//    juce::Atomic<int> size = 0;
//
//    void pushNextSampleIntoFifo(float sample)
//    {
//        if (fifoIndex == bufferToFill.getNumSamples())
//        {
//            auto ok = audioBufferFifo.push(bufferToFill);
//
//            juce::ignoreUnused(ok);
//
//            fifoIndex = 0;
//        }
//
//        bufferToFill.setSample(0, fifoIndex, sample);
//        ++fifoIndex;
//    }
//};

struct ChainSettings
{
    float lowShelfFreq { 100.0f }, lowShelfDecibels { 0.0f }, lowShelfQ { 1.0f };
    float band1Freq { 200.0f}, band1Decibels { 0.0f }, band1Q { 1.0f };
    float band2Freq { 500.0f }, band2Decibels { 0.0f }, band2Q { 1.0f };
    float band3Freq { 1000.0f }, band3Decibels { 0.0f }, band3Q { 1.0f };
    float band4Freq { 1500.0f }, band4Decibels { 0.0f }, band4Q { 1.0f };
    float band5Freq { 2500.0f }, band5Decibels { 0.0f }, band5Q { 1.0f };
    float highShelfFreq { 3000.0f }, highShelfDecibels { 0.0f }, highShelfQ { 1.0f };
};

ChainSettings getChainSettings(juce::AudioProcessorValueTreeState& apvts);

using Filter = juce::dsp::IIR::Filter<float>;
using MonoChain = juce::dsp::ProcessorChain<Filter, Filter, Filter, Filter, Filter, Filter, Filter>;

enum ChainPositions
{
    lowShelf,
    band1,
    band2,
    band3,
    band4,
    band5,
    highShelf
};

using Coefficients = Filter::CoefficientsPtr;
void updateCoefficients(Coefficients& old, const Coefficients& replacements);
Coefficients createFilter(const ChainSettings& settings, double sampleRate, int position);

//==============================================================================
/**
*/
class BassDivisionAudioProcessor  : public juce::AudioProcessor, public juce::AudioProcessorValueTreeState::Listener,
public juce::ValueTree::Listener
{
public:
    //==============================================================================
    BassDivisionAudioProcessor();
    ~BassDivisionAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    juce::AudioProcessorValueTreeState treeState;
    
    void updateFilters(const ChainSettings& chainSettings);
    
    PresetManager* getPresetManager();
    ImpulseResponseManager* getImpulseResponseManager();
    
    void setImpulseResponseIsEngaged(bool impulseResponseEngaged) { this->isImpulseResponseEngaged = impulseResponseEngaged; }
    
    ///TODO: Previous iteration had a complicated method for determining if a plug was being loaded for the first time, and thus if a parameter being changed was due to an interface interaction or not. This meant that an * would be added to the end of a preset name when a parameter was changed, indicating that it had changed since last load. It was up to the user to save if that was their desire!
    
    // This was done with a set and get on a bool parameter. Likely this was because it was interacting outside of the processor class. This appears to no longer be the case, and could probably be a private member variable that gets updated internally.
    
    
    using BlockType = juce::AudioBuffer<float>;
    SingleChannelSampleFifo<BlockType> leftChannelFifo { Channel::Left };
    SingleChannelSampleFifo<BlockType> rightChannelFifo { Channel::Right };
    
    juce::Value sampleRate;

    SpectrumProcessor* getSpectrumProcessor() { return &spectrumProcessor; }

private:
    juce::TimeSliceThread renderThread;
    SpectrumProcessor spectrumProcessor;
    
    PresetManager presetManager;
    ImpulseResponseManager impulseResponseManager;
    
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    void parameterChanged(const juce::String& parameterID, float newValue) override;
    void setInitialParameters();
    
    juce::Atomic<bool> onLoad { false };

    // EQ and bands
    MonoChain leftChain, rightChain;
    
    juce::dsp::LinkwitzRileyFilter<float> subLowPass, lowHighPass, lowLowPass, midhighPass, midLowPass, highHighPass, irHighPass, irLowPass;

    // Processing for crossovers
    void processSubBandCrossover(juce::dsp::ProcessContextReplacing<float> context);
    void processLowBandCrossover(juce::dsp::ProcessContextReplacing<float> context);
    void processMidBandCrossover(juce::dsp::ProcessContextReplacing<float> context);
    void processHighBandCrossover(juce::dsp::ProcessContextReplacing<float> context);
    void processIrHighpass(juce::dsp::ProcessContextReplacing<float> context);
    void processIrLowpass(juce::dsp::ProcessContextReplacing<float> context);

    // Compressor
    DivisionVoid::Compressor<float> compressor;

    float compInputGain = 1.0f;
    float compOutputGain = 1.0f;

    //Sub bass processing
    DivisionVoid::Compressor<float> subCompressor;
    
    float subInputGainLin = 1.0f;
    float subOutputGainLin = 1.0f;

    // Low Processing
    DivisionVoid::Compressor<float> lowCompressor;

    float lowInputGainLin = 1.0f;
    float lowOutputGainLin = 1.0f;

    // Mid Processing
    DivisionVoid::Distortion<float> midDistortion;

    // High Processing
    DivisionVoid::Distortion<float> highDistortion;

    // Impulse Response
    juce::dsp::Convolution impulseResponse;
    juce::File currentImpulseResponse;

    // IO
    float inputGain = 1.0f;
    float outputGain = 1.0f;

    // Band Buffers
    juce::AudioBuffer<float> subBuffer, lowBuffer, midBuffer, highBuffer, irBuffer, cleanBuffer;

    // Smoothed values for meters
    juce::LinearSmoothedValue<float> dBLeftIn, dBRightIn;
    
    // Bools for processing
    bool subSoloOn = false;
    bool lowSoloOn = false;
    bool midSoloOn = false;
    bool highSoloOn = false;
    bool isImpulseResponseEngaged = false;
    
    // Crossover setters and getters
    void setLowCrossover(float freq);
    void setMidCrossover(float freq);
    void setHighCrossover(float freq);
    
    // DSP processing context
    void processIr(juce::dsp::ProcessContextReplacing<float> context);
    void processIrCrossover(juce::AudioBuffer<float>& buffer);
    void processCrossover(juce::AudioBuffer<float>& buffer);
    
    void setImpulseResponse(juce::File ir);
    juce::File irReference;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BassDivisionAudioProcessor)
};
