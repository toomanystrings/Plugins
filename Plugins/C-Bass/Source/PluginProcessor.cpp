//
// Created by Conor Foran on 14/08/2025.
//

#include "PluginProcessor.h"
#include "PluginEditor.h"

CBassAudioProcessor::CBassAudioProcessor() :
    apvts(*this, nullptr, "Parameters", createParameterLayout()),
    os(getTotalNumOutputChannels(), osFactor, juce::dsp::Oversampling<float>::FilterType::filterHalfBandPolyphaseIIR, false)
{
    for (auto p : getParameters())
        apvts.addParameterListener(static_cast<juce::AudioProcessorParameterWithID*>(p)->paramID, this);
}

void CBassAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                       juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused(midiMessages);
    juce::ScopedNoDenormals noDenormals;

    auto numChannels = buffer.getNumChannels();
    auto numSamples = buffer.getNumSamples();

    juce::dsp::AudioBlock<float> block(buffer);

    // Sanity check
    for (int channel = 0; channel < numChannels; ++channel)
    {
        auto* data = buffer.getWritePointer(channel);

        for (int i = 0; i < numSamples; ++i)
            if (!std::isfinite(data[i]))
                data[i] = 0.0f;
    }

    dryBuffer.makeCopyOf(buffer);

    tempBuffer.makeCopyOf(buffer);

    juce::dsp::AudioBlock<float> tempBlock(tempBuffer);
    juce::dsp::ProcessContextReplacing<float> bpContext(tempBlock);

    // Band pass filter on wet path
    bandpassFilter.process(bpContext);

    // Copy the band buffer, so we can subtract the original band
    bandBuffer.makeCopyOf(tempBuffer);

    // Oversample up
    auto upsampledBlock = os.processSamplesUp(tempBlock);
    auto osNumSamples = upsampledBlock.getNumSamples();
    auto osNumChannels = upsampledBlock.getNumChannels();

    // Wave shaping to introduce harmonics
    float drive = juce::jlimit(1.0f, 20.0f, std::exp(intensity * 1.5f));
    float norm = 1.0f / std::tanh(drive);;

    for (int channel = 0; channel < osNumChannels; ++channel)
    {
        auto* data = upsampledBlock.getChannelPointer(channel);

        for (int sample = 0; sample < osNumSamples; ++sample)
        {
            float x = data[sample] * drive * 0.3f;
            x = juce::jlimit(-5.0f, 5.0f, x);

            float y;
            if (x >= 0.0f)
                y = juce::dsp::FastMathApproximations::tanh(x) * norm;
            else
                y = 0.6f * juce::dsp::FastMathApproximations::tanh(x) * norm;

            if (!std::isfinite(y))
                y = 0.0f;

            data[sample] = y * 0.7f;
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

    // High and low filters pass to tame output on wave-shaped signal
    juce::dsp::ProcessContextReplacing<float> lpContext(tempBlock);
    lowpassFilter.process(lpContext);

    juce::dsp::ProcessContextReplacing<float> hpContext(tempBlock);
    highpassFilter.process(hpContext);

    // Recombine with original signal. Commented out so that just the wet path can be analysed.
    float wetMix = std::sqrtf(intensity);

    for (int channel = 0; channel < numChannels; ++channel)
    {
        auto* dry = dryBuffer.getReadPointer(channel);
        auto* wet = tempBuffer.getReadPointer(channel);
        auto* out = buffer.getWritePointer(channel);

        for (int i = 0; i < numSamples; ++i)
        {
            float y = dry[i] + wet[i] * wetMix;

            // Final safety clamp
            if (!std::isfinite(y))
                y = 0.0f;

            out[i] = y;
        }
    }
}

void CBassAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    auto numChannels = getTotalNumOutputChannels();
    juce::dsp::ProcessSpec spec;

    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = numChannels;

    dryBuffer.setSize(numChannels, samplesPerBlock);
    dryBuffer.clear();

    bandBuffer.setSize(numChannels, samplesPerBlock);
    bandBuffer.clear();

    tempBuffer.setSize(numChannels, samplesPerBlock);
    tempBuffer.clear();

    bandpassFilter.state = juce::dsp::IIR::Coefficients<float>::makeBandPass(sampleRate, 200.0f, 0.8f);
    bandpassFilter.prepare(spec);

    highpassFilter.state = juce::dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, 150.0f);
    highpassFilter.prepare(spec);

    lowpassFilter.state = juce::dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, 250.0f);
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

    parameters.push_back(MakeUnique<juce::AudioParameterFloat>("Gain", "Gain", 0.0f, 1.0f, 1.0f));
    parameters.push_back(MakeUnique<juce::AudioParameterFloat>("Intensity", "Intensity", 0.0f, 1.0f, 0.5f));
    parameters.push_back(MakeUnique<juce::AudioParameterFloat>("Band", "Band", 100.0f, 280.0f, 120.0f));

    return { parameters.begin(), parameters.end() };
}

void CBassAudioProcessor::parameterChanged (const String &parameterID, float newValue)
{
    if (parameterID == "Gain")
        gain = newValue;

    if (parameterID == "Intensity")
        intensity = newValue;

    if (parameterID == "Band")
        centreFreq = newValue;
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new CBassAudioProcessor();
}
