/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BassDivisionAudioProcessor::BassDivisionAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), treeState (*this, nullptr, "PARAMETERS", createParameterLayout()), sampleRate(44100), renderThread("FFT Render Thread"), spectrumProcessor(11), presetManager(this), impulseResponseManager(this)
#endif
{
    for (auto p : getParameters())
        treeState.addParameterListener(static_cast<juce::AudioProcessorParameterWithID*>(p)->paramID, this);
    
    compressor.engageHPF(true);
    subCompressor.engageHPF(false);
    lowCompressor.engageHPF(true);
    lowCompressor.engageHarmonicContent(true);
    lowCompressor.setHPFFreq(200);
    
    renderThread.addTimeSliceClient(&spectrumProcessor);
    renderThread.startThread(3);
}

BassDivisionAudioProcessor::~BassDivisionAudioProcessor()
{
    for (auto p : getParameters())
        treeState.removeParameterListener(static_cast<juce::AudioProcessorParameterWithID*>(p)->paramID, this);
    
    renderThread.removeTimeSliceClient(&spectrumProcessor);
    renderThread.stopThread(500);
}

//==============================================================================
const juce::String BassDivisionAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool BassDivisionAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool BassDivisionAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool BassDivisionAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double BassDivisionAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int BassDivisionAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int BassDivisionAudioProcessor::getCurrentProgram()
{
    return 0;
}

void BassDivisionAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String BassDivisionAudioProcessor::getProgramName (int index)
{
    return {};
}

void BassDivisionAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void BassDivisionAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    auto numChannels = getTotalNumOutputChannels();
    
    juce::dsp::ProcessSpec spec;
    juce::dsp::ProcessSpec monoSpec;
    
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = numChannels;
    
    monoSpec.sampleRate = sampleRate;
    monoSpec.maximumBlockSize = samplesPerBlock;
    monoSpec.numChannels = 1;
    
    subBuffer.setSize(numChannels, samplesPerBlock);
    lowBuffer.setSize(numChannels, samplesPerBlock);
    midBuffer.setSize(numChannels, samplesPerBlock);
    highBuffer.setSize(numChannels, samplesPerBlock);
    irBuffer.setSize(numChannels, samplesPerBlock);
    cleanBuffer.setSize(numChannels, samplesPerBlock);

    subBuffer.clear();
    lowBuffer.clear();
    midBuffer.clear();
    highBuffer.clear();
    irBuffer.clear();
    cleanBuffer.clear();

    compressor.setSampleRate(sampleRate);
    subCompressor.setSampleRate(sampleRate);
    lowCompressor.setSampleRate(sampleRate);

    midDistortion.prepare(sampleRate, samplesPerBlock);
    highDistortion.prepare(sampleRate, samplesPerBlock);

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
    
    subLowPass.setType(juce::dsp::LinkwitzRileyFilterType::lowpass);
    lowLowPass.setType(juce::dsp::LinkwitzRileyFilterType::lowpass);
    midLowPass.setType(juce::dsp::LinkwitzRileyFilterType::lowpass);
    irLowPass.setType(juce::dsp::LinkwitzRileyFilterType::lowpass);

    lowHighPass.setType(juce::dsp::LinkwitzRileyFilterType::highpass);
    midhighPass.setType(juce::dsp::LinkwitzRileyFilterType::highpass);
    highHighPass.setType(juce::dsp::LinkwitzRileyFilterType::highpass);
    irHighPass.setType(juce::dsp::LinkwitzRileyFilterType::highpass);
    
    setInitialParameters();
    
    leftChannelFifo.prepare(samplesPerBlock);
    rightChannelFifo.prepare(samplesPerBlock);
    
    spectrumProcessor.setSampleRate(sampleRate);
}

void BassDivisionAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool BassDivisionAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void BassDivisionAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    auto bufferSize = buffer.getNumSamples();
    auto numChannels = buffer.getNumChannels();
    
    onLoad.compareAndSetBool(false, true);
    
    
    
    setImpulseResponse(impulseResponseManager.getCurrentImpulseResponse());
    
    buffer.applyGain(inputGain);
    
    juce::dsp::AudioBlock<float> block(buffer);
    
    auto leftBlock = block.getSingleChannelBlock(Channel::Left);
    auto rightBlock = block.getSingleChannelBlock(Channel::Right);
    
    juce::dsp::ProcessContextReplacing<float> leftContext(leftBlock);
    juce::dsp::ProcessContextReplacing<float> rightContext(rightBlock);
    
    leftChain.process(leftContext);
    rightChain.process(rightContext);
    
    buffer.applyGain(compInputGain);

    for (int channel = 0; channel < numChannels; ++channel)
        compressor.processChannel(buffer.getWritePointer(channel), channel, bufferSize);
    
    buffer.applyGain(compOutputGain);
    
    processCrossover(buffer);

    subBuffer.applyGain(subInputGainLin);

    for (int channel = 0; channel < numChannels; ++channel)
        subCompressor.processChannel(subBuffer.getWritePointer(channel), channel, bufferSize);

    subBuffer.applyGain(subOutputGainLin);

    lowBuffer.applyGain(lowInputGainLin);

    for (int channel = 0; channel < numChannels; ++channel)
        lowCompressor.processChannel(lowBuffer.getWritePointer(channel), channel, bufferSize);

    lowBuffer.applyGain(lowOutputGainLin);

    for (int channel = 0; channel < numChannels; ++channel)
        midDistortion.process(midBuffer.getWritePointer(channel), channel, bufferSize);
    
    for (int channel = 0; channel < numChannels; ++channel)
        highDistortion.process(highBuffer.getWritePointer(channel), channel, bufferSize);

    for (int channel = 0; channel < numChannels; ++channel)
    {
        auto xSub = subBuffer.getWritePointer(channel);
        auto xLow = lowBuffer.getWritePointer(channel);
        auto xMid = midBuffer.getWritePointer(channel);
        auto xHigh = highBuffer.getWritePointer(channel);

        auto xOut = buffer.getWritePointer(channel);

        // Recombine the signals. This feels clunky
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
                xOut[sample] = xSub[sample] + xLow[sample] + xMid[sample] + xHigh[sample];
        }
    }
    
    if (isImpulseResponseEngaged)
    {
        processIrCrossover(buffer);
        
        juce::dsp::AudioBlock<float> block(irBuffer);
        processIr(block);

        for (int channel = 0; channel < numChannels; ++channel)
        {
            auto xIr = irBuffer.getWritePointer(channel);
            auto xClean = cleanBuffer.getWritePointer(channel);
            
            auto xOut = buffer.getWritePointer(channel);

            for (int sample = 0; sample < bufferSize; ++sample)
                xOut[sample] = xIr[sample] + xClean[sample];
        }
    }
    
    
    buffer.applyGain(outputGain);
    
    //leftChannelFifo.update(buffer);
    //rightChannelFifo.update(buffer);
    
    for (int channel = 0; channel < totalNumOutputChannels; ++channel)
    {
        auto channelData = buffer.getReadPointer(channel);
        if (getActiveEditor() != nullptr
            && channel == 0)
        {
            spectrumProcessor.copySamples(channelData, bufferSize);
        }
    }
            
    
    //juce::AudioPlayHead* playHead = getPlayHead();

    //juce::AudioPlayHead::CurrentPositionInfo pos;
    //playHead->getCurrentPosition(pos);
    
    //auto isPlaying = pos.isPlaying;
    //if (getActiveEditor() != nullptr)
    //{
        // This is where code was being passed from processor to interface, but only when the interface was open.
        // Perhaps not as good a move as I believed?
        
        // Have found a way to by and large not have to pass from here. Might be useful to
    //}
}

//==============================================================================
bool BassDivisionAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* BassDivisionAudioProcessor::createEditor()
{
    return new BassDivisionAudioProcessorEditor (*this);
}

//==============================================================================
void BassDivisionAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // Make a local copy of APVTS
    auto state = treeState.copyState();

    // Get the current preset name from preset manager
    const auto currentPresetName = presetManager.getCurrentPresetName();
    const auto currentImpulseResponseName = impulseResponseManager.getCurrentImpulseResponseName();

    // If that preset name is valid, add it to the tree before turning it into an xml element and copying it to binary
    if (currentPresetName.isNotEmpty())
        state.setProperty(juce::Identifier("PresetName"), currentPresetName, nullptr);

    if (currentImpulseResponseName.isNotEmpty())
        state.setProperty(juce::Identifier("IRName"), currentImpulseResponseName, nullptr);

    std::unique_ptr<juce::XmlElement> xml(state.createXml());

    copyXmlToBinary(*xml, destData);
}

void BassDivisionAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.

    // Having issues here with loading the preset back up. When the preset name gets called, it overrides the
    // actual last settings if the preset was moved in any way. Forcing the user to save the preset before closing is
    // not a good idea. Maybe a flag can be made so that it knows not to assign a flag? Not sure how to handle this...

    // Take the incoming data and turn it into an XmlElement
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    // If that xml data is valid, create a tree with it, and check if it has a PresetName propertry.
    // If so, set the presetManager's currentPreset name to the value found there, then remove the
    // preset name property and copy it to the processor's state
    if(xmlState.get() != nullptr)
    {
        if(xmlState->hasTagName(treeState.state.getType()))
        {
            // Copy the tree that is being restored
            auto newTree = juce::ValueTree::fromXml(*xmlState);
            
            auto presetID = juce::Identifier("PresetName");
            auto irID = juce::Identifier("IRName");

            //juce::String presetString;
                
            if(newTree.hasProperty(presetID))
            {
                //presetString = newTree.getPropertyAsValue("PresetName", nullptr).toString();
                presetManager.setCurrentPresetName(newTree.getPropertyAsValue("PresetName", nullptr).toString());
                newTree.removeProperty(presetID, nullptr);

                onLoad.compareAndSetBool(true, false);
            }

            if(newTree.hasProperty(irID))
            {
                const auto ir = juce::File(impulseResponseManager.getCurrentImpulseResponseDirectory() + directorySeperator
                    + newTree.getPropertyAsValue("IRName", nullptr).toString() + ".wav");
                    
                //impulseResponseManager.loadImpulseResponse(ir);
                impulseResponseManager.setCurrentImpulseResponseName(newTree.getPropertyAsValue("IRName", nullptr).toString());

                setImpulseResponse(impulseResponseManager.getCurrentImpulseResponse());
                    
                newTree.removeProperty(irID, nullptr);
            }

            // Replace the current state with the loaded one
            treeState.replaceState(newTree);

            //presetManager.setCurrentPresetName(presetString);
        }
    }
}

PresetManager* BassDivisionAudioProcessor::getPresetManager()
{
    return &presetManager;
}

ImpulseResponseManager* BassDivisionAudioProcessor::getImpulseResponseManager()
{
    return &impulseResponseManager;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new BassDivisionAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout BassDivisionAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    /*******************************************************************
     * COMPRESSOR
     *******************************************************************/

    auto ioCompNormRange = juce::NormalisableRange<float>(-24.f, 12.f, 0.01f);
    ioCompNormRange.setSkewForCentre(0);

    auto compAttackRange = juce::NormalisableRange<float>(20, 500, 1);
    compAttackRange.setSkewForCentre(100);
    
    auto compReleaseRange = juce::NormalisableRange<float>(20, 1000, 1);
    compReleaseRange.setSkewForCentre(250);
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>("COMP_ATTACK", "Comp Attack", compAttackRange, 100));    // Need to tweak these values so that they are being passed in ms, then converted to s in the algorithm
    params.push_back(std::make_unique<juce::AudioParameterFloat>("COMP_RELEASE", "Comp Release", compReleaseRange, 250));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("COMP_RATIO", "Comp Ratio", 1.f, 10.f, 4.f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("COMP_THRESHOLD", "Comp Threshold", -64.f, 0.f, -12.f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("COMP_KNEE", "Comp Knee", 1, 10, 1));
    params.push_back(std::make_unique<juce::AudioParameterInt>("COMP_HPF", "Comp HPF", 20, 500, 200));

    params.push_back(std::make_unique<juce::AudioParameterFloat>("COMP_INPUT", "Comp Input", ioCompNormRange, 0.f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("COMP_OUTPUT", "Comp Output", ioCompNormRange, 0.f));

    /*******************************************************************
     * SUB COMPRESSOR
     *******************************************************************/

    params.push_back(std::make_unique<juce::AudioParameterFloat>("SUB_COMP_ATTACK", "Sub Attack", compAttackRange, 100));    // Need to tweak these values so that they are being passed in ms, then converted to s in the algorithm
    params.push_back(std::make_unique<juce::AudioParameterFloat>("SUB_COMP_RELEASE", "Sub Release", compReleaseRange, 250));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("SUB_COMP_RATIO", "Sub Ratio", 1.f, 10.f, 4.f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("SUB_COMP_THRESHOLD", "Sub Threshold", -64.f, 0.f, -12.f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>("SUB_COMP_INPUT", "Sub Input", ioCompNormRange, 0.f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("SUB_COMP_OUTPUT", "Sub Output", ioCompNormRange, 0.f));

    /*******************************************************************
     * LOW COMPRESSOR
     *******************************************************************/

    params.push_back(std::make_unique<juce::AudioParameterFloat>("LOW_COMP_ATTACK", "Low Attack", compAttackRange, 100));    // Need to tweak these values so that they are being passed in ms, then converted to s in the algorithm
    params.push_back(std::make_unique<juce::AudioParameterFloat>("LOW_COMP_RELEASE", "Low Release", compReleaseRange, 250));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("LOW_COMP_RATIO", "Low Ratio", 1.f, 10.f, 4.f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("LOW_COMP_THRESHOLD", "Low Threshold", -64.f, 0.f, -12.f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("LOW_COMP_INPUT", "Low Input", ioCompNormRange, 0.f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("LOW_COMP_OUTPUT", "Low Output", ioCompNormRange, 0.f));

    /*******************************************************************
     * MID DISTORTION
     *******************************************************************/

    auto distGainNormRange = juce::NormalisableRange<float>(-24.f, 12.f, 0.01f);
    distGainNormRange.setSkewForCentre(0);

    juce::StringArray types;
    types.add("Tube-Screamer");
    types.add("MXR+");
    types.add("Cubic");
    types.add("Hard Square");
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>("MID_DISTORTION_GAIN", "Mid Gain", distGainNormRange, 0.f));
    params.push_back(std::make_unique<juce::AudioParameterInt>("MID_DISTORTION_WETDRY", "Mid Wet/Dry", 0, 100, 50));
    params.push_back(std::make_unique<juce::AudioParameterChoice>("MID_DISTORTION_TYPE", "Mid Type", types, 0));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("MID_DISTORTION_EDGE", "Mid Edge", 1.f, 10.f, 1.f));

    /*******************************************************************
     * HIGH DISTORTION
     *******************************************************************/

    params.push_back(std::make_unique<juce::AudioParameterFloat>("HIGH_DISTORTION_GAIN", "High Gain", distGainNormRange, 0.f));
    params.push_back(std::make_unique<juce::AudioParameterInt>("HIGH_DISTORTION_WETDRY", "High Wet/Dry", 0, 100, 50));
    params.push_back(std::make_unique<juce::AudioParameterChoice>("HIGH_DISTORTION_TYPE", "High Type", types, 0));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("HIGH_DISTORTION_EDGE", "High Edge", 1.f, 10.f, 1.f));

    /*******************************************************************
     * CROSSOVER BANDS
     *******************************************************************/

    params.push_back(std::make_unique<juce::AudioParameterInt>("LOW_XOVER", "Low crossover", 40, 80, 60));
    params.push_back(std::make_unique<juce::AudioParameterInt>("MID_XOVER", "Mid crossover", 200, 400, 300));
    params.push_back(std::make_unique<juce::AudioParameterInt>("HIGH_XOVER", "High crossover", 1200, 2000, 1500));

    /*******************************************************************
     * IO
     *******************************************************************/

    auto ioNormRange = juce::NormalisableRange<float>(-24.f, 12.f, 0.01f);
    //ioNormRange.setSkewForCentre(0);

    params.push_back(std::make_unique<juce::AudioParameterFloat>("INPUT", "Input", ioNormRange, 0.f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("OUTPUT", "Output", ioNormRange, 0.f));

    /*******************************************************************
     * EQ
     *******************************************************************/

    params.push_back(std::make_unique<juce::AudioParameterFloat>("EQ_LOW_SHELF", "Low shelf", -12.f, 12.f, 0.f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("EQ_BAND_1", "200 Hz", -12.f, 12.f, 0.f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("EQ_BAND_2", "500 Hz", -12.f, 12.f, 0.f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("EQ_BAND_3", "1 kHz", -12.f, 12.f, 0.f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("EQ_BAND_4", "1.5 kHz", -12.f, 12.f, 0.f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("EQ_BAND_5", "2.5 kHz", -12.f, 12.f, 0.f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("EQ_HIGH_SHELF", "High shelf", -12.f, 12.f, 0.f));

    /*******************************************************************
     * SOLO BUTTONS
     *******************************************************************/

    params.push_back(std::make_unique<juce::AudioParameterBool>("SUB_SOLO", "Sub Solo", false));
    params.push_back(std::make_unique<juce::AudioParameterBool>("LOW_SOLO", "Low Solo", false));
    params.push_back(std::make_unique<juce::AudioParameterBool>("MID_SOLO", "Mid Solo", false));
    params.push_back(std::make_unique<juce::AudioParameterBool>("HIGH_SOLO", "High Solo", false));
    
    params.push_back(std::make_unique<juce::AudioParameterBool>("IR_ENABLED", "IR Enabled", false));
    
    return { params.begin(), params.end() };
}

void BassDivisionAudioProcessor::parameterChanged(const juce::String& parameterID, float newValue)
{
    /*******************************************************************
     * SUB COMPRESSOR
     *******************************************************************/
    
    if (parameterID == "SUB_COMP_ATTACK")
        subCompressor.setAttack(newValue / 1000);

    if (parameterID == "SUB_COMP_RELEASE")
        subCompressor.setRelease(newValue / 1000);

    if (parameterID == "SUB_COMP_RATIO")
        subCompressor.setRatio(newValue);

    if (parameterID == "SUB_COMP_THRESHOLD")
        subCompressor.setThreshold(newValue);

    if (parameterID == "SUB_COMP_INPUT")
        subInputGainLin = juce::Decibels::decibelsToGain(newValue);

    if (parameterID == "SUB_COMP_OUTPUT")
        subOutputGainLin = juce::Decibels::decibelsToGain(newValue);

    /*******************************************************************
     * LOW ... SOMETHING
     *******************************************************************/

    if (parameterID == "LOW_COMP_ATTACK")
        lowCompressor.setAttack(newValue / 1000);

    if (parameterID == "LOW_COMP_RELEASE")
        lowCompressor.setRelease(newValue / 1000);

    if (parameterID == "LOW_COMP_RATIO")
        lowCompressor.setRatio(newValue);

    if (parameterID == "LOW_COMP_THRESHOLD")
        lowCompressor.setThreshold(newValue);

    if (parameterID == "LOW_COMP_INPUT")
        lowInputGainLin = juce::Decibels::decibelsToGain(newValue);

    if (parameterID == "LOW_COMP_OUTPUT")
        lowOutputGainLin = juce::Decibels::decibelsToGain(newValue);

    /*******************************************************************
     * MID DISTORTION
     *******************************************************************/

    if (parameterID == "MID_DISTORTION_GAIN")
        midDistortion.setInputGain(newValue);

    if (parameterID == "MID_DISTORTION_WETDRY")
        midDistortion.setMix(newValue);

    if (parameterID == "MID_DISTORTION_TYPE")
        midDistortion.setType(static_cast<int>(newValue));

    if (parameterID == "MID_DISTORTION_EDGE")
        midDistortion.setEdge(newValue);

    /*******************************************************************
     * HIGH DISTORTION
     *******************************************************************/

    if (parameterID == "HIGH_DISTORTION_GAIN")
        highDistortion.setInputGain(newValue);

    if (parameterID == "HIGH_DISTORTION_WETDRY")
        highDistortion.setMix(newValue);

    if (parameterID == "HIGH_DISTORTION_TYPE")
        highDistortion.setType(static_cast<int>(newValue));

    if (parameterID == "HIGH_DISTORTION_EDGE")
        highDistortion.setEdge(newValue);

    /*******************************************************************
     * IO
     *******************************************************************/

    if (parameterID == "INPUT")
        inputGain = juce::Decibels::decibelsToGain(newValue);

    if (parameterID == "OUTPUT")
        outputGain = juce::Decibels::decibelsToGain(newValue);

    /*******************************************************************
     * COMPRESSOR
     *******************************************************************/

    if (parameterID == "COMP_ATTACK")
        compressor.setAttack(newValue / 1000);

    if (parameterID == "COMP_RELEASE")
        compressor.setRelease(newValue / 1000);

    if (parameterID == "COMP_RATIO")
        compressor.setRatio(newValue);

    if (parameterID == "COMP_THRESHOLD")
        compressor.setThreshold(newValue);

    if (parameterID == "COMP_KNEE")
        compressor.setKnee(newValue);

    if (parameterID == "COMP_HPF")
        compressor.setHPFFreq(newValue);

    if (parameterID == "COMP_INPUT")
        compInputGain = juce::Decibels::decibelsToGain(newValue);

    if (parameterID == "COMP_OUTPUT")
        compOutputGain = juce::Decibels::decibelsToGain(newValue);
    

    /*******************************************************************
     * EQ
     *******************************************************************/
    
    auto settings = getChainSettings(treeState);

    if (parameterID == "EQ_LOW_SHELF")
        updateFilters(settings);
        
    if (parameterID == "EQ_BAND_1")
        updateFilters(settings);

    if (parameterID == "EQ_BAND_2")
        updateFilters(settings);
        
    if (parameterID == "EQ_BAND_3")
        updateFilters(settings);
        
    if (parameterID == "EQ_BAND_4")
        updateFilters(settings);
        
    if (parameterID == "EQ_BAND_5")
        updateFilters(settings);
       
    if (parameterID == "EQ_HIGH_SHELF")
        updateFilters(settings);
        

    /*******************************************************************
     * CROSSOVERS
     *******************************************************************/

    if (parameterID == "LOW_XOVER")
        setLowCrossover(newValue);

    if (parameterID == "MID_XOVER")
        setMidCrossover(newValue);

    if (parameterID == "HIGH_XOVER")
        setHighCrossover(newValue);

    /*******************************************************************
     * SOLO BUTTONS
     *******************************************************************/

    if (parameterID == "SUB_SOLO")
        subSoloOn = static_cast<bool>(newValue);
    
    if (parameterID == "LOW_SOLO")
        lowSoloOn = static_cast<bool>(newValue);

    if (parameterID == "MID_SOLO")
        midSoloOn = static_cast<bool>(newValue);

    if (parameterID == "HIGH_SOLO")
        highSoloOn = static_cast<bool>(newValue);

    if (parameterID == "IR_ENABLED")
        isImpulseResponseEngaged = static_cast<bool>(newValue);

    if (onLoad.value == false)
    {
        if (!presetManager.getCurrentPresetName().containsChar('*'))
        {
            presetManager.setCurrentPresetName(presetManager.getCurrentPresetName() + '*');
        }
    }
}

void BassDivisionAudioProcessor::setInitialParameters()
{
    /*******************************************************************
     * CROSSOVER
     *******************************************************************/

    auto lowCrossover = treeState.getRawParameterValue("LOW_XOVER")->load();
    auto midCrossover = treeState.getRawParameterValue("MID_XOVER")->load();
    auto highCrossover = treeState.getRawParameterValue("HIGH_XOVER")->load();
    
    setLowCrossover(lowCrossover);
    setMidCrossover(midCrossover);
    setHighCrossover(highCrossover);

    // Create a method to set the ir crossover filters.

    /*******************************************************************
     * SUB COMPRESSOR VALUES
     *******************************************************************/

    auto subCompAttack = treeState.getRawParameterValue("SUB_COMP_ATTACK")->load();
    auto subCompRelease = treeState.getRawParameterValue("SUB_COMP_RELEASE")->load();
    auto subCompRatio = treeState.getRawParameterValue("SUB_COMP_RATIO")->load();
    auto subCompThresh = treeState.getRawParameterValue("SUB_COMP_THRESHOLD")->load();

    subCompressor.setAttack(subCompAttack / 1000);
    subCompressor.setRelease(subCompRelease / 1000);
    subCompressor.setRatio(subCompRatio);
    subCompressor.setThreshold(subCompThresh);

    /*******************************************************************
     * LOW COMPRESSOR VALUES
     *******************************************************************/

    auto lowCompAttack = treeState.getRawParameterValue("LOW_COMP_ATTACK")->load();
    auto lowCompRelease = treeState.getRawParameterValue("LOW_COMP_RELEASE")->load();
    auto lowCompRatio = treeState.getRawParameterValue("LOW_COMP_RATIO")->load();
    auto lowCompThresh = treeState.getRawParameterValue("LOW_COMP_THRESHOLD")->load();

    lowCompressor.setAttack(lowCompAttack / 1000);
    lowCompressor.setRelease(lowCompRelease / 1000);
    lowCompressor.setRatio(lowCompRatio);
    lowCompressor.setThreshold(lowCompThresh);

    /*******************************************************************
     * DISTORTION VALUES - MIDS
     *******************************************************************/

    auto midGain = treeState.getRawParameterValue("MID_DISTORTION_GAIN")->load();
    auto midType = treeState.getRawParameterValue("MID_DISTORTION_TYPE")->load();
    auto midEdge = treeState.getRawParameterValue("MID_DISTORTION_EDGE")->load();
    auto midMix = treeState.getRawParameterValue("MID_DISTORTION_WETDRY")->load();

    midDistortion.setEdge(midEdge);
    midDistortion.setType(midType);
    midDistortion.setInputGain(midGain);
    midDistortion.setMix(midMix);

    /*******************************************************************
     * DISTORTION VALUES - HIGHS
     *******************************************************************/

    auto highGain = treeState.getRawParameterValue("HIGH_DISTORTION_GAIN")->load();
    auto highType = treeState.getRawParameterValue("HIGH_DISTORTION_TYPE")->load();
    auto highEdge = treeState.getRawParameterValue("HIGH_DISTORTION_EDGE")->load();
    auto highMix = treeState.getRawParameterValue("HIGH_DISTORTION_WETDRY")->load();

    highDistortion.setEdge(highEdge);
    highDistortion.setType(highType);
    highDistortion.setInputGain(highGain);
    highDistortion.setMix(highMix);

    /*******************************************************************
     * EQ VALUES
     *******************************************************************/

    auto settings = getChainSettings(treeState);

    updateFilters(settings);
    
    /*******************************************************************
     * COMPRESSOR VALUES
     *******************************************************************/

    auto compAttack = treeState.getRawParameterValue("COMP_ATTACK")->load();
    auto compRelease = treeState.getRawParameterValue("COMP_RELEASE")->load();
    auto compRatio = treeState.getRawParameterValue("COMP_RATIO")->load();
    auto compThresh = treeState.getRawParameterValue("COMP_THRESHOLD")->load();
    auto compKnee = treeState.getRawParameterValue("COMP_KNEE")->load();
    auto compHpf = treeState.getRawParameterValue("COMP_HPF")->load();

    compressor.setAttack(compAttack / 1000);
    compressor.setRelease(compRelease / 1000);
    compressor.setRatio(compRatio);
    compressor.setThreshold(compThresh);
    compressor.setKnee(compKnee);
    compressor.setHPFFreq(compHpf);

}

void BassDivisionAudioProcessor::processSubBandCrossover(juce::dsp::ProcessContextReplacing<float> context)
{
    subLowPass.process(context);
}

void BassDivisionAudioProcessor::processLowBandCrossover(juce::dsp::ProcessContextReplacing<float> context)
{
    lowLowPass.process(context);
    lowHighPass.process(context);
}

void BassDivisionAudioProcessor::processMidBandCrossover(juce::dsp::ProcessContextReplacing<float> context)
{
    midLowPass.process(context);
    midhighPass.process(context);
}

void BassDivisionAudioProcessor::processHighBandCrossover(juce::dsp::ProcessContextReplacing<float> context)
{
    highHighPass.process(context);
}

void BassDivisionAudioProcessor::processIrHighpass(juce::dsp::ProcessContextReplacing<float> context)
{
    irHighPass.process(context);
}

void BassDivisionAudioProcessor::processIrLowpass(juce::dsp::ProcessContextReplacing<float> context)
{
    irLowPass.process(context);
}

void BassDivisionAudioProcessor::setLowCrossover(float freq)
{
    subLowPass.setCutoffFrequency(freq);
    lowHighPass.setCutoffFrequency(freq);
}

void BassDivisionAudioProcessor::setMidCrossover(float freq)
{
    lowLowPass.setCutoffFrequency(freq);
    midhighPass.setCutoffFrequency(freq);
}

void BassDivisionAudioProcessor::setHighCrossover(float freq)
{
    midLowPass.setCutoffFrequency(freq);
    highHighPass.setCutoffFrequency(freq);
}

void BassDivisionAudioProcessor::processIr(juce::dsp::ProcessContextReplacing<float> context)
{
    impulseResponse.process(context);
}

void BassDivisionAudioProcessor::processIrCrossover(juce::AudioBuffer<float>& buffer)
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

void BassDivisionAudioProcessor::processCrossover(juce::AudioBuffer<float>& buffer)
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

void BassDivisionAudioProcessor::setImpulseResponse(juce::File ir)
{
    juce::File temp = ir;
    
    if (temp != irReference)
    {
        irReference = temp;
        if (impulseResponse.getCurrentIRSize() == 0)
        {
            impulseResponse.loadImpulseResponse(temp, juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::yes, temp.getSize());
        }
        else if (impulseResponse.getCurrentIRSize() != 0)
        {
            impulseResponse.loadImpulseResponse(temp, juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::yes, temp.getSize());
        }
    }
}

ChainSettings getChainSettings(juce::AudioProcessorValueTreeState& apvts)
{
    ChainSettings settings;
    
    settings.lowShelfDecibels = apvts.getRawParameterValue("EQ_LOW_SHELF")->load();
    settings.band1Decibels = apvts.getRawParameterValue("EQ_BAND_1")->load();
    settings.band2Decibels = apvts.getRawParameterValue("EQ_BAND_2")->load();
    settings.band3Decibels = apvts.getRawParameterValue("EQ_BAND_3")->load();
    settings.band4Decibels = apvts.getRawParameterValue("EQ_BAND_4")->load();
    settings.band5Decibels = apvts.getRawParameterValue("EQ_BAND_5")->load();
    settings.highShelfDecibels = apvts.getRawParameterValue("EQ_HIGH_SHELF")->load();
    
    return settings;
}

Coefficients createFilter(const ChainSettings& settings, double sampleRate, int position)
{
    switch (position)
    {
        case 0:
            return juce::dsp::IIR::Coefficients<float>::makeLowShelf(sampleRate, settings.lowShelfFreq, settings.lowShelfQ, juce::Decibels::decibelsToGain(settings.lowShelfDecibels));
            break;
        case 1:
            return juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, settings.band1Freq, settings.band1Q, juce::Decibels::decibelsToGain(settings.band1Decibels));
            break;
        case 2:
            return juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, settings.band2Freq, settings.band2Q, juce::Decibels::decibelsToGain(settings.band2Decibels));
            break;
        case 3:
            return juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, settings.band3Freq, settings.band3Q, juce::Decibels::decibelsToGain(settings.band3Decibels));
            break;
        case 4:
            return juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, settings.band4Freq, settings.band4Q, juce::Decibels::decibelsToGain(settings.band4Decibels));
            break;
        case 5:
            return juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, settings.band5Freq, settings.band5Q, juce::Decibels::decibelsToGain(settings.band5Decibels));
            break;
        case 6:
            return juce::dsp::IIR::Coefficients<float>::makeHighShelf(sampleRate, settings.highShelfFreq, settings.highShelfQ, juce::Decibels::decibelsToGain(settings.highShelfDecibels));
            break;
    }
}

void BassDivisionAudioProcessor::updateFilters(const ChainSettings& settings)
{
    auto lowShelfCoeffs = createFilter(settings, getSampleRate(), ChainPositions::lowShelf);
    auto band1Coeffs = createFilter(settings, getSampleRate(), ChainPositions::band1);
    auto band2Coeffs = createFilter(settings, getSampleRate(), ChainPositions::band2);
    auto band3Coeffs = createFilter(settings, getSampleRate(), ChainPositions::band3);
    auto band4Coeffs = createFilter(settings, getSampleRate(), ChainPositions::band4);
    auto band5Coeffs = createFilter(settings, getSampleRate(), ChainPositions::band5);
    auto highShelfCoeffs = createFilter(settings, getSampleRate(), ChainPositions::highShelf);
    
    updateCoefficients(leftChain.get<ChainPositions::lowShelf>().coefficients, lowShelfCoeffs);
    updateCoefficients(rightChain.get<ChainPositions::lowShelf>().coefficients, lowShelfCoeffs);
    
    updateCoefficients(leftChain.get<ChainPositions::band1>().coefficients, band1Coeffs);
    updateCoefficients(rightChain.get<ChainPositions::band1>().coefficients, band1Coeffs);
    
    updateCoefficients(leftChain.get<ChainPositions::band2>().coefficients, band2Coeffs);
    updateCoefficients(rightChain.get<ChainPositions::band2>().coefficients, band2Coeffs);
    
    updateCoefficients(leftChain.get<ChainPositions::band3>().coefficients, band3Coeffs);
    updateCoefficients(rightChain.get<ChainPositions::band3>().coefficients, band3Coeffs);
    
    updateCoefficients(leftChain.get<ChainPositions::band4>().coefficients, band4Coeffs);
    updateCoefficients(rightChain.get<ChainPositions::band4>().coefficients, band4Coeffs);
    
    updateCoefficients(leftChain.get<ChainPositions::band5>().coefficients, band5Coeffs);
    updateCoefficients(rightChain.get<ChainPositions::band5>().coefficients, band5Coeffs);
    
    updateCoefficients(leftChain.get<ChainPositions::highShelf>().coefficients, highShelfCoeffs);
    updateCoefficients(rightChain.get<ChainPositions::highShelf>().coefficients, highShelfCoeffs);
}

void updateCoefficients(Coefficients &old, const Coefficients &replacements)
{
    *old = *replacements;
}
