//
// Created by Conor Foran on 14/08/2025.
//

#include "PluginProcessor.h"
#include "PluginEditor.h"

CBassAudioProcessor::CBassAudioProcessor() :
    apvts(*this, nullptr, "Parameters", createParameterLayout()),
    os(getTotalNumOutputChannels(), osFactor, juce::dsp::Oversampling<float>::FilterType::filterHalfBandFIREquiripple)
{
    for (auto p : getParameters())
        apvts.addParameterListener(static_cast<juce::AudioProcessorParameterWithID*>(p)->paramID, this);
}

void CBassAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                       juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused(midiMessages);

    auto numChannels = buffer.getNumChannels();
    auto numSamples = buffer.getNumSamples();

    // Set up the processing for the dsp block
    juce::dsp::AudioBlock<float> block(buffer);

    bandBuffer.makeCopyOf(buffer);
    tempBuffer.makeCopyOf(buffer);

    juce::dsp::AudioBlock<float> bandBlock(bandBuffer);
    juce::dsp::AudioBlock<float> tempBlock(tempBuffer);

    juce::dsp::ProcessContextReplacing<float> bpContext(bandBlock);

    // Band pass filter on wet path
    bandpassFilter.process(bpContext);

    tempBuffer.makeCopyOf(bandBuffer);

    // Oversample up
    auto upsampledBlock = os.processSamplesUp(tempBlock);
    auto osNumSamples = upsampledBlock.getNumSamples();
    auto osNumChannels = upsampledBlock.getNumChannels();

    // Wave shaping to introduce harmonics
    for (int channel = 0; channel < osNumChannels; ++channel)
    {
        auto* data = upsampledBlock.getChannelPointer(channel);
        for (int sample = 0; sample < osNumSamples; ++sample)
        {
            data[sample] = std::tanh(data[sample]);
        }
    }

    // Oversample down
    os.processSamplesDown(tempBlock);

    // Isolate harmonics
    for (int channel = 0; channel < numChannels; ++channel)
    {
        auto* shaped = tempBuffer.getWritePointer(channel);
        auto* band= bandBuffer.getReadPointer(channel);

        for (int sample = 0; sample < numSamples; ++sample)
            shaped[sample] = shaped[sample] - band[sample];
    }

    // High and low filters pass to tame output
    juce::dsp::ProcessContextReplacing<float> lpContext(tempBlock);
    lowpassFilter.process(lpContext);

    juce::dsp::ProcessContextReplacing<float> hpContext(tempBlock);
    highpassFilter.process(hpContext);

    // Recombine with original signal. Commented out so that just the wet path can be analysed.
    for (int channel = 0; channel < numChannels; ++channel)
    {
        auto* dry = buffer.getWritePointer(channel);
        auto* wet = tempBuffer.getReadPointer(channel);

        for (int sample = 0; sample < numSamples; ++sample)
            dry[sample] += wet[sample];// * intensity;
    }
}

void CBassAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    auto numChannels = getTotalNumOutputChannels();
    juce::dsp::ProcessSpec spec;

    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = numChannels;

    bandBuffer.setSize(numChannels, samplesPerBlock);
    bandBuffer.clear();

    tempBuffer.setSize(numChannels, samplesPerBlock);
    tempBuffer.clear();

    bandpassFilter.state = juce::dsp::IIR::Coefficients<float>::makeBandPass(sampleRate, 120.0f, 0.8f);
    bandpassFilter.prepare(spec);

    highpassFilter.state = juce::dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, 80.0f);
    highpassFilter.prepare(spec);

    lowpassFilter.state = juce::dsp::IIR::Coefficients<float>::makeFirstOrderLowPass(sampleRate, 200.0f);
    lowpassFilter.prepare(spec);

    os.reset();
    os.initProcessing(samplesPerBlock);

    setLatencySamples(os.getLatencyInSamples());
}

juce::AudioProcessorEditor* CBassAudioProcessor::createEditor()
{
    return new CBassAudioProcessorEditor(*this);
    //return new juce::GenericAudioProcessorEditor(*this);
}

void CBassAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    //Serializes your parameters, and any other potential data into an XML:

    //auto params = PluginHelpers::saveParamsTree(*this);

    //auto pluginPreset = juce::ValueTree(getName());
    //pluginPreset.appendChild(params, nullptr);
    //This a good place to add any non-parameters to your preset

    //copyXmlToBinary(*pluginPreset.createXml(), destData);
}

void CBassAudioProcessor::setStateInformation(const void* data,
                                              int sizeInBytes)
{
    //Loads your parameters, and any other potential data from an XML:

    //if (auto xml = getXmlFromBinary(data, sizeInBytes))
    //{
    //   auto preset = juce::ValueTree::fromXml(*xml);
    //    auto params = preset.getChildWithName("Params");

    //    PluginHelpers::loadParamsTree(*this, params);

        //Load your non-parameter data now
    //}
}

juce::AudioProcessorValueTreeState::ParameterLayout CBassAudioProcessor::createParameterLayout ()
{
    std::vector<UniquePtr<juce::RangedAudioParameter>> parameters;

    parameters.push_back(MakeUnique<juce::AudioParameterFloat>("Gain", "Gain", 0.0f, 1.0f, 0.5f));
    parameters.push_back(MakeUnique<juce::AudioParameterFloat>("Intensity", "Intensity", 0.0f, 1.0f, 0.5f));
    parameters.push_back(MakeUnique<juce::AudioParameterFloat>("Band", "Band", 20.0f, 100.0f, 40.0f));

    return { parameters.begin(), parameters.end() };
}

void CBassAudioProcessor::parameterChanged (const String &parameterID, float newValue)
{

}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new CBassAudioProcessor();
}
