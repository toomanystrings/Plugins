/*
  ==============================================================================

    BDProcessor.h
    Created: 23 Feb 2021 11:22:14pm
    Author:  Olorin

    In this class we will collate all the processors that were going on in the plugin processor and allow the plugin to process
    buffer by buffer.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "Compressor.h"
#include "BDDistortion.h"
#include "FIFO.h"

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

ChainSettings getChainSettings();

class BDProcessor
{
public:
	BDProcessor();
    ~BDProcessor();
    
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

    void processBuffer(juce::AudioBuffer<float> buffer);

    void prepare(juce::dsp::ProcessSpec spec, juce::dsp::ProcessSpec monoSpec);

	// Sub bass setters and getters
    void setSubAttack(float attack);
    void setSubRelease(float release);
    void setSubRatio(float ratio);
    void setSubThreshold(float threshold);
    void setSubInput(float inputGain);
    void setSubOutput(float outputGain);

    // Low setters and getters
    void setLowAttack(float attack);
    void setLowRelease(float release);
    void setLowRatio(float ratio);
    void setLowThreshold(float threshold);
    void setLowInput(float inputGain);
    void setLowOutput(float outputGain);
    void setLowDrive(float drive);

    // Mids setters and getters
    void setMidGain(float gain);
    void setMidEdge(float edge);
    void setMidType(int type);
    void setMidMix(float mix);
	
    // Highs setters and getters
    void setHighGain(float gain);
    void setHighEdge(float edge);
    void setHighType(int type);
    void setHighMix(float mix);

    // EQ setters and getters
    void setEqGain(ChainPositions, float gain, float freq);
    void setEqFilters(juce::Array<float> gains);

	// Compressor setters and getters
    void setCompAttack(float attack);
    void setCompRelease(float release);
    void setCompRatio(float ratio);
    void setCompThreshold(float threshold);
    void setCompKnee(float knee);
    void setCompHpf(float freq);
    void setCompInput(float inputGain);
    void setCompOutput(float outputGain);
	
    // Crossover setters and getters
    void setLowCrossover(float freq);
    void setMidCrossover(float freq);
    void setHighCrossover(float freq);
    void setBandFilterTypes();

    // IO setters and getters
    void setInputGain(float inputGain);
    void setOutputGain(float outputGain);

	// Impulse Response functions
    void setImpulseResponse(juce::File ir);
    void setIRReference(juce::File ir) { this->currentImpulseResponse = ir; }

	// DSP processing context
    void processIr(juce::dsp::ProcessContextReplacing<float> context);
    void processIrCrossover(juce::AudioBuffer<float>& buffer);
    void processCrossover(juce::AudioBuffer<float>& buffer);

	// Solo buttons
    void subSoloEngaged(bool soloOn);
    void lowSoloEngaged(bool soloOn);
    void midSoloEngaged(bool soloOn);
    void highSoloEngaged(bool soloOn);
    void implulseResponseEngaged(bool isImpulseResponseEngaged);

private:
    // General
    double sampleRate = 44100;
    int numChannel = 1;

    // EQ and band passes
    
    using Filter = juce::dsp::IIR::Filter<float>;
    using MonoChain = juce::dsp::ProcessorChain<Filter, Filter, Filter, Filter, Filter, Filter, Filter>;
    
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
    Compressor compressor;

    float compInputGain = 1.0f;
    float compOutputGain = 1.0f;

	//Sub bass processing
    Compressor subCompressor;
    
    float subInputGainLin = 1.0f;
    float subOutputGainLin = 1.0f;

	// Low Processing
    Compressor lowCompressor;

    float lowInputGainLin = 1.0f;
    float lowOutputGainLin = 1.0f;

	// Mid Processing
    DivisionVoid::Distortion midDistortion;

	// High Processing
	DivisionVoid::Distortion highDistortion;

	// Impulse Response
    juce::dsp::Convolution impulseResponse;
    juce::File currentImpulseResponse;

	// IO
    float inputGain = 1.0f;
    float outputGain = 1.0f;

	// Band Buffers
    juce::AudioBuffer<float> subBuffer, lowBuffer, midBuffer, highBuffer, irBuffer, cleanBuffer;

	// Bools for processing
    bool subSoloOn = false;
    bool lowSoloOn = false;
    bool midSoloOn = false;
    bool highSoloOn = false;
    bool isImpulseResponseEngaged = false;

	// Frequencies for the EQ bank
    juce::Array<float> freqs = { 100.0f, 200.0f, 500.0f, 1000.0f, 1500.0f, 2500.0f, 3000.0f };

    // Arrays for passing values to the UI
    
    float* sumStereo(juce::AudioBuffer<float> buffer);
    
    
};
