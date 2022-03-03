/*
  ==============================================================================

    IrManager.h
    Created: 12 Nov 2020 6:47:30pm
    Author:  Olorin

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Utilities.h"



class ImpulseResponseManager
{
public:
	ImpulseResponseManager(juce::AudioProcessor* p);

	void loadImpulseResponse(juce::File irToLoad);
	void clearImpulseResponse();

	const juce::String getImpulseResponseName(int inIrIndex);

	const juce::String getCurrentImpulseResponseName();

	void setCurrentImpulseResponseName(juce::String irName);

	const juce::String getCurrentImpulseResponseDirectory();

	juce::File getCurrentImpulseResponse();

	bool isImpulseResponseLoaded();

	int getNumberOfImpulseResponses();

private:
	void updateImpulseResponseList();

	juce::File currentImpulseResponse;
	juce::File noIRLoaded;

	juce::File impulseResponseDirectory;

	juce::String currentImpulseResponseName{ "No IR Loaded" };

	juce::Array<juce::File> impulseResponseList;

	bool ImpulseResponseLoaded = false;

	//BassDivisionAudioProcessor& audioProcessor;
	juce::AudioProcessor* audioProcessor;
};
