/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ADTAudioProcessor::ADTAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), treeState(*this, nullptr, "PARAMETERS", createParameterLayout())
#endif
{
    for (auto p : getParameters())
        treeState.addParameterListener(static_cast<juce::AudioProcessorParameterWithID*>(p)->paramID, this);
}

ADTAudioProcessor::~ADTAudioProcessor()
{
    for (auto p : getParameters())
        treeState.removeParameterListener(static_cast<juce::AudioProcessorParameterWithID*>(p)->paramID, this);
}

//==============================================================================
const juce::String ADTAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ADTAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ADTAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ADTAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ADTAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ADTAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ADTAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ADTAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String ADTAudioProcessor::getProgramName (int index)
{
    return {};
}

void ADTAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void ADTAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    auto numChannels = getTotalNumOutputChannels();



    //delayLine.setMaximumDelayInSamples(50000);
    //dryLine.setMaximumDelayInSamples(.07 * sampleRate);
    
    // wetBuffer might need to be a different size. At the very least, we will need to implement a read and write head
    wetBuffer.setSize(numChannels, samplesPerBlock);
    dryBuffer.setSize(numChannels, samplesPerBlock);
  
    wetBuffer.clear();
    dryBuffer.clear();
}

void ADTAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ADTAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void ADTAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    auto bufferSize = buffer.getNumSamples();
    auto numChannels = buffer.getNumChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // Analysis of the signal

    // Get the RMS value of the signal
    auto rms = buffer.getRMSLevel(0, 0, bufferSize);

    // Effective range to be decided, but at the minute lets aim for -30ms to 30ms
    // We need to bind the rms value to within those two limits
    // RMS is never going to be more than 1, but will rarely actually get much above 0.5
    auto delayInSamples = AdtDSP::msToSamples(getSampleRate(), 800);


    auto dryDelay = 0.05 * Fs;

    // Might want to do more streamlined processing than sample by sample, but let's prove the concept.
    for (auto channel = 0; channel < numChannels; ++channel)
    {
        auto data = buffer.getWritePointer(channel);

        for (auto sample = 0; sample < bufferSize; ++sample)
        {
            dryLine.pushSample(channel, data[sample]);
            data[sample] = dryLine.popSample(channel, dryDelay);

            delayLine.pushSample(channel, data[sample]);
            data[sample] += delayLine.popSample(channel, delayInSamples);
        }
    }
}

//==============================================================================
bool ADTAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ADTAudioProcessor::createEditor()
{
    //return new ADTAudioProcessorEditor (*this);
    return new juce::GenericAudioProcessorEditor (*this);
}

//==============================================================================
void ADTAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void ADTAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ADTAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout ADTAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back(MakeUnique<juce::AudioParameterFloat>("DELAY", "Time Shift", 0, 10, 5));
    params.push_back(MakeUnique<juce::AudioParameterFloat>("LEVEL", "Level Variation", 0, 10, 5));
    params.push_back(MakeUnique<juce::AudioParameterFloat>("PITCH", "Pitch Shift", 0 ,10, 5));

    return {params.begin(), params.end()};
}

void ADTAudioProcessor::parameterChanged(const juce::String &parameterID, float newValue)
{
    if (parameterID == "DELAY")
    {
        delayLine.setDelay(newValue * 5000);
    }
}
