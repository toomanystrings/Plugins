//
// Created by Conor Foran on 14/08/2025.
//

#include "PluginProcessor.h"
#include "PluginEditor.h"

CBassAudioProcessor::CBassAudioProcessor() :
    apvts(*this, nullptr, "Parameters", createParameterLayout())
{
    for (auto p : getParameters())
        apvts.addParameterListener(static_cast<juce::AudioProcessorParameterWithID*>(p)->paramID, this);
}

void CBassAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                       juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused(midiMessages);
}

void CBassAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    ProcessorBase::prepareToPlay(sampleRate, samplesPerBlock);
}

juce::AudioProcessorEditor* CBassAudioProcessor::createEditor()
{
    //return new CBassAudioProcessorEditor(*this);
    return new juce::GenericAudioProcessorEditor(*this);
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
