/*
  ==============================================================================

    BDProcessor.cpp
    Created: 23 Feb 2021 11:22:14pm
    Author:  Olorin

  ==============================================================================
*/

#include "BDProcessor.h"

BDProcessor::BDProcessor()
{
	compressor.engageHPF(true);
	subCompressor.engageHPF(false);
	lowCompressor.engageHPF(true);
	lowCompressor.engageHarmonicContent(true);
	lowCompressor.setHPFFreq(200);
}

BDProcessor::~BDProcessor()
{
}

// This method takes a buffer and overwrites the samples, and as such returns no values. Pass a buffer and
// listen to the magic.
void BDProcessor::processBuffer(juce::AudioBuffer<float> buffer)
{
	// Some basic variables for processing
	auto bufferSize = buffer.getNumSamples();
	auto numChannels = buffer.getNumChannels();
	
	/*******************************************************************
	 * INPUT
	 *
	 * First off, apply some gain, and store values in the relevant FIFOs
	 *******************************************************************/
	buffer.applyGain(inputGain);

	/*******************************************************************
	 * EQ
	 *
	 * Passes the buffer through juce IIR filters
	 *******************************************************************/
    juce::dsp::AudioBlock<float> block(buffer);
    
    auto leftBlock = block.getSingleChannelBlock(0);
    auto rightBlock = block.getSingleChannelBlock(1);
    
    juce::dsp::ProcessContextReplacing<float> leftContext(leftBlock);
    juce::dsp::ProcessContextReplacing<float> rightContext(rightBlock);
    
    leftChain.process(leftContext);
    rightChain.process(rightContext);
	
	/*******************************************************************
	 * COMPRESSOR
	 *
	 * Apply some gain, compress, and pass values to various FIFOs
	 *******************************************************************/
	buffer.applyGain(compInputGain);

	for (int channel = 0; channel < numChannels; ++channel)
		compressor.processChannel(buffer.getWritePointer(channel), channel, bufferSize);
	
	buffer.applyGain(compOutputGain);
	
	/*******************************************************************
	 * CROSSOVER
	 *
	 * Splits the buffer into four separate buffers, ready for further
	 * processing.
	 *******************************************************************/
	processCrossover(buffer);

	/*******************************************************************
	 * SUB
	 *
	 * The sub and the lows are the same, and very similar to the overall
	 * compressor.
	 *******************************************************************/
	subBuffer.applyGain(subInputGainLin);

	for (int channel = 0; channel < numChannels; ++channel)
		subCompressor.processChannel(subBuffer.getWritePointer(channel), channel, bufferSize);

	subBuffer.applyGain(subOutputGainLin);

	/*******************************************************************
	 * LOW
	 *******************************************************************/
	lowBuffer.applyGain(lowInputGainLin);

	for (int channel = 0; channel < numChannels; ++channel)
		lowCompressor.processChannel(lowBuffer.getWritePointer(channel), channel, bufferSize);

	lowBuffer.applyGain(lowOutputGainLin);

	/*******************************************************************
	 * MID
	 *
	 * Mids and highs are the same. Distort the signal.
	 *******************************************************************/
	for (int channel = 0; channel < numChannels; ++channel)
		midDistortion.process(midBuffer.getWritePointer(channel), channel, bufferSize);

	/*******************************************************************
	 * HIGH
	 *******************************************************************/
	for (int channel = 0; channel < numChannels; ++channel)
		highDistortion.process(highBuffer.getWritePointer(channel), channel, bufferSize);


	/*******************************************************************
	 * RECOMBINE
	 *
	 * Recombine the signals before we finish up
	 *******************************************************************/
	for (int channel = 0; channel < numChannels; ++channel)
	{
		// Next up, get write pointers for the channels
		auto xSub = subBuffer.getWritePointer(channel);
		auto xLow = lowBuffer.getWritePointer(channel);
		auto xMid = midBuffer.getWritePointer(channel);
		auto xHigh = highBuffer.getWritePointer(channel);

		auto xOut = buffer.getWritePointer(channel);

		// Recombine the signals
		if (subSoloOn || lowSoloOn || midSoloOn || highSoloOn)
		{
			for (int sample = 0; sample < bufferSize; ++sample)
				xOut[sample] = xSub[sample] * static_cast<int>(subSoloOn)
				+ xLow[sample] * static_cast<int>(lowSoloOn)
				+ xMid[sample] * static_cast<int>(midSoloOn)
				+ xHigh[sample] * static_cast<int>(highSoloOn);
		}
		else
		{
			for (int sample = 0; sample < bufferSize; ++sample)
				xOut[sample] = xSub[sample]	+ xLow[sample] + xMid[sample] + xHigh[sample];
		}
		
	}

	/*******************************************************************
	 * IMPULSE RESPONSE
	 *******************************************************************/
	if (isImpulseResponseEngaged)
	{
		processIrCrossover(buffer);
		
		juce::dsp::AudioBlock<float> block(irBuffer);
		processIr(block);

		for (int channel = 0; channel < numChannels; ++channel)
		{
			// Next up, get write pointers for the channels
			auto xIr = irBuffer.getWritePointer(channel);
			auto xClean = cleanBuffer.getWritePointer(channel);
			
			auto xOut = buffer.getWritePointer(channel);

			for (int sample = 0; sample < bufferSize; ++sample)
				xOut[sample] = xIr[sample] + xClean[sample];
		}			
	}
	
	/*******************************************************************
	 * OUTPUT
	 *******************************************************************/
	buffer.applyGain(outputGain);
}

// Prepare the class for processing
void BDProcessor::prepare(juce::dsp::ProcessSpec spec, juce::dsp::ProcessSpec monoSpec)
{
	int numChannels = spec.numChannels;
    double sampleRate = spec.sampleRate;
    int bufferSize = spec.maximumBlockSize;
	
    subBuffer.setSize(numChannels, bufferSize);
    lowBuffer.setSize(numChannels, bufferSize);
	midBuffer.setSize(numChannels, bufferSize);
	highBuffer.setSize(numChannels, bufferSize);
	irBuffer.setSize(numChannels, bufferSize);
	cleanBuffer.setSize(numChannels, bufferSize);

	subBuffer.clear();
	lowBuffer.clear();
	midBuffer.clear();
	highBuffer.clear();
	irBuffer.clear();
	cleanBuffer.clear();

	compressor.setSampleRate(sampleRate);
	subCompressor.setSampleRate(sampleRate);
	lowCompressor.setSampleRate(sampleRate);

	midDistortion.prepare(sampleRate, bufferSize);
	highDistortion.prepare(sampleRate, bufferSize);

	this->sampleRate = sampleRate;

	subLowPass.prepare(spec);
	lowHighPass.prepare(spec);
	lowLowPass.prepare(spec);
	midhighPass.prepare(spec);
	midLowPass.prepare(spec);
	highHighPass.prepare(spec);
	irHighPass.prepare(spec);
	irLowPass.prepare(spec);

	irHighPass.setCutoffFrequency(500);
	irLowPass.setCutoffFrequency(500);

	impulseResponse.prepare(spec);
    
    leftChain.prepare(monoSpec);
    rightChain.prepare(monoSpec);
    
    //auto chainSettings = getChainSettings(treeState);
}

void BDProcessor::setSubAttack(float attack)
{
	subCompressor.setAttack(attack);
}

void BDProcessor::setSubRelease(float release)
{
	subCompressor.setRelease(release);
}

void BDProcessor::setSubRatio(float ratio)
{
	subCompressor.setRatio(ratio);
}

void BDProcessor::setSubThreshold(float threshold)
{
	subCompressor.setThreshold(threshold);
}

void BDProcessor::setSubInput(float inputGain)
{
	this->subInputGainLin = inputGain;
}

void BDProcessor::setSubOutput(float outputGain)
{
	this->subOutputGainLin = outputGain;
}

void BDProcessor::setLowAttack(float attack)
{
	lowCompressor.setAttack(attack);
}

void BDProcessor::setLowRelease(float release)
{
	lowCompressor.setRelease(release);
}

void BDProcessor::setLowRatio(float ratio)
{
	lowCompressor.setRatio(ratio);
}

void BDProcessor::setLowThreshold(float threshold)
{
	lowCompressor.setThreshold(threshold);
}

void BDProcessor::setLowInput(float inputGain)
{
	this->lowInputGainLin = inputGain;
}

void BDProcessor::setLowOutput(float outputGain)
{
	this->lowOutputGainLin = outputGain;
}

void BDProcessor::setLowDrive(float drive)
{
	lowCompressor.setDrive(drive);
}

void BDProcessor::setMidGain(float gain)
{
	midDistortion.setInputGain(gain);
}

void BDProcessor::setMidEdge(float edge)
{
	midDistortion.setEdge(edge/10);
}

void BDProcessor::setMidType(int type)
{
	midDistortion.setType(type);
}

void BDProcessor::setMidMix(float mix)
{
	midDistortion.setMix(mix);
}

void BDProcessor::setHighGain(float gain)
{
	highDistortion.setInputGain(gain);
}

void BDProcessor::setHighEdge(float edge)
{
	highDistortion.setEdge(edge/10);
}

void BDProcessor::setHighType(int type)
{
	highDistortion.setType(type);
}

void BDProcessor::setHighMix(float mix)
{
	highDistortion.setMix(mix);
}

// Set the gain for a filter in an array of filters
void BDProcessor::setEqGain(ChainPositions chainPosition, float gain, float freq)
{
//    auto chainSettings = getChainSettings();
//
//
//
//    switch (chainPosition)
//    {
//        case lowShelf:
//            auto coefficients = juce::dsp::IIR::Coefficients<float>::makeLowShelf(sampleRate, chainSettings.lowShelfFreq, chainSettings.lowShelfQ, juce::Decibels::decibelsToGain(gain));
//            break;
//
//        case highShelf:
//            auto coefficients = juce::dsp::IIR::Coefficients<float>::makeHighShelf(sampleRate, chainSettings.highShelfFreq, chainSettings.highShelfQ, juce::Decibels::decibelsToGain(gain));
//            break;
//            
//        case band1:
//            auto coefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, chainSettings.band1Freq, chainSettings.band1Q, juce::Decibels::decibelsToGain(gain));
//            break;
//    }
//
//
//
}

void BDProcessor::setCompAttack(float attack)
{
	compressor.setAttack(attack);
}

void BDProcessor::setCompRelease(float release)
{
	compressor.setRelease(release);
}

void BDProcessor::setCompRatio(float ratio)
{
	compressor.setRatio(ratio);
}

void BDProcessor::setCompThreshold(float threshold)
{
	compressor.setThreshold(threshold);
}

void BDProcessor::setCompKnee(float knee)
{
	compressor.setKnee(knee);
}

void BDProcessor::setCompHpf(float freq)
{
	compressor.setHPFFreq(freq);
}

void BDProcessor::setCompInput(float inputGain)
{
	this->compInputGain = inputGain;
}

void BDProcessor::setCompOutput(float outputGain)
{
	this->compOutputGain = outputGain;
}

void BDProcessor::setLowCrossover(float freq)
{
		subLowPass.setCutoffFrequency(freq);
		lowHighPass.setCutoffFrequency(freq);
}

void BDProcessor::setMidCrossover(float freq)
{
		lowLowPass.setCutoffFrequency(freq);
		midhighPass.setCutoffFrequency(freq);
}

void BDProcessor::setHighCrossover(float freq)
{
		midLowPass.setCutoffFrequency(freq);
		highHighPass.setCutoffFrequency(freq);
}

void BDProcessor::setInputGain(float inputGain)
{
	this->inputGain = inputGain;
}

void BDProcessor::setOutputGain(float outputGain)
{
	this->outputGain = outputGain;
}

void BDProcessor::setImpulseResponse(juce::File ir)
{
	juce::File IR = ir;

	if (IR != currentImpulseResponse)
	{
		setIRReference(IR);
		if (impulseResponse.getCurrentIRSize() == 0)
		{
			impulseResponse.loadImpulseResponse(IR, juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::yes, IR.getSize());
		}
		else if (impulseResponse.getCurrentIRSize() != 0)
		{
			impulseResponse.loadImpulseResponse(IR, juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::yes, IR.getSize());
		}
	}
}

void BDProcessor::processIr(juce::dsp::ProcessContextReplacing<float> context)
{
	impulseResponse.process(context);
}

void BDProcessor::processIrCrossover(juce::AudioBuffer<float>& buffer)
{
	for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
	{
		irBuffer.copyFrom(channel, 0, buffer, channel, 0, buffer.getNumSamples());
		cleanBuffer.copyFrom(channel, 0, buffer, channel, 0, buffer.getNumSamples());
	}
	
	juce::dsp::AudioBlock<float> irBlock(irBuffer);
	processIrHighpass(irBlock);

	juce::dsp::AudioBlock<float> cleanBlock(cleanBuffer);
	processIrLowpass(cleanBlock);
}

void BDProcessor::processCrossover(juce::AudioBuffer<float>& buffer)
{
	for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
	{
		subBuffer.copyFrom(channel, 0, buffer, channel, 0, buffer.getNumSamples());
		lowBuffer.copyFrom(channel, 0, buffer, channel, 0, buffer.getNumSamples());
		midBuffer.copyFrom(channel, 0, buffer, channel, 0, buffer.getNumSamples());
		highBuffer.copyFrom(channel, 0, buffer, channel, 0, buffer.getNumSamples());
	}
		
	juce::dsp::AudioBlock<float> subBlock(subBuffer);
	processSubBandCrossover(subBlock);
		
	juce::dsp::AudioBlock<float> lowBlock(lowBuffer);
	processLowBandCrossover(lowBlock);
		
	juce::dsp::AudioBlock<float> midBlock(midBuffer);
	processMidBandCrossover(midBlock);
		
	juce::dsp::AudioBlock<float> highBlock(highBuffer);
	processHighBandCrossover(highBlock);
}

void BDProcessor::setBandFilterTypes()
{
		subLowPass.setType(juce::dsp::LinkwitzRileyFilterType::lowpass);
		lowLowPass.setType(juce::dsp::LinkwitzRileyFilterType::lowpass);
		midLowPass.setType(juce::dsp::LinkwitzRileyFilterType::lowpass);
		irLowPass.setType(juce::dsp::LinkwitzRileyFilterType::lowpass);

		lowHighPass.setType(juce::dsp::LinkwitzRileyFilterType::highpass);
		midhighPass.setType(juce::dsp::LinkwitzRileyFilterType::highpass);
		highHighPass.setType(juce::dsp::LinkwitzRileyFilterType::highpass);
		irHighPass.setType(juce::dsp::LinkwitzRileyFilterType::highpass);
}

void BDProcessor::setEqFilters(juce::Array<float> gains)
{
//	for (int channel = 0; channel < 2; ++channel)
//	{
//		for (int filter = 0; filter < 7; ++filter)
//		{
//			if (filter == 0)
//				filterBank[channel][filter].setCoefficients(juce::IIRCoefficients::makeLowShelf(sampleRate, freqs[filter], 0.707f, gains[filter]));
//			else if (filter == 6)
//				filterBank[channel][filter].setCoefficients(juce::IIRCoefficients::makeHighShelf(sampleRate, freqs[filter], 0.707f, gains[filter]));
//			else
//				filterBank[channel][filter].setCoefficients(juce::IIRCoefficients::makePeakFilter(sampleRate, freqs[filter], 0.707f, gains[filter]));
//		}
//	}
}

void BDProcessor::processSubBandCrossover(juce::dsp::ProcessContextReplacing<float> context)
{
	subLowPass.process(context);
}

void BDProcessor::processLowBandCrossover(juce::dsp::ProcessContextReplacing<float> context)
{
	lowLowPass.process(context);
	lowHighPass.process(context);
}

void BDProcessor::processMidBandCrossover(juce::dsp::ProcessContextReplacing<float> context)
{
	midLowPass.process(context);
	midhighPass.process(context);
}

void BDProcessor::processHighBandCrossover(juce::dsp::ProcessContextReplacing<float> context)
{
	highHighPass.process(context);
}

void BDProcessor::processIrHighpass(juce::dsp::ProcessContextReplacing<float> context)
{
	irHighPass.process(context);
}

void BDProcessor::processIrLowpass(juce::dsp::ProcessContextReplacing<float> context)
{
	irLowPass.process(context);
}

void BDProcessor::subSoloEngaged(bool soloOn)
{
	this->subSoloOn = soloOn;
}

void BDProcessor::lowSoloEngaged(bool soloOn)
{
	this->lowSoloOn = soloOn;
}

void BDProcessor::midSoloEngaged(bool soloOn)
{
	this->midSoloOn = soloOn;
}

void BDProcessor::highSoloEngaged(bool soloOn)
{
	this->highSoloOn = soloOn;
}

void BDProcessor::implulseResponseEngaged(bool isImpulseResponseEngaged)
{
	this->isImpulseResponseEngaged = isImpulseResponseEngaged;
}


float* BDProcessor::sumStereo(juce::AudioBuffer<float> buffer)
{
	const int numSamples = buffer.getNumSamples();

	std::vector<float> stereoArray;

	for (int sample = 0; sample < numSamples; ++sample)
		stereoArray.push_back(0.5 * (buffer.getReadPointer(0)[sample] + buffer.getReadPointer(1)[sample]));

	return stereoArray.data();
}

ChainSettings getChainSettings()
{
    ChainSettings settings;
    
//    settings.lowShelfDecibels = treeState.getRawParameterValue("EQ_LOW_SHELF")->load();
//    settings.band1Decibels = treeState.getRawParameterValue("EQ_BAND_1")->load();
//    settings.band1Decibels = treeState.getRawParameterValue("EQ_BAND_2")->load();
//    settings.band1Decibels = treeState.getRawParameterValue("EQ_BAND_3")->load();
//    settings.band1Decibels = treeState.getRawParameterValue("EQ_BAND_4")->load();
//    settings.band1Decibels = treeState.getRawParameterValue("EQ_BAND_5")->load();
//    settings.highShelfDecibels = treeState.getRawParameterValue("EQ_HIGH_SHELF")->load();
    
    return settings;
}


