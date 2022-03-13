/*
  ==============================================================================

    IrManager.cpp
    Created: 12 Nov 2020 6:47:30pm
    Author:  Olorin

  ==============================================================================
*/

#include "IrManager.h"

ImpulseResponseManager::ImpulseResponseManager(juce::AudioProcessor* p) : audioProcessor(p)
{
	impulseResponseDirectory = juce::File(juce::File::getSpecialLocation(juce::File::userDocumentsDirectory).getFullPathName()
											+ directorySeperator /*"\\"*/ + "Tote Bag"
											+ directorySeperator /*"\\"*/ + audioProcessor->getName()
											+ directorySeperator /*"\\"*/ + "IRs");

	noIRLoaded = juce::File();
	
	if (!impulseResponseDirectory.exists())
		impulseResponseDirectory.createDirectory();
	
	updateImpulseResponseList();
}

void ImpulseResponseManager::loadImpulseResponse(juce::File irToLoad)
{
	juce::MemoryBlock blockToLoadFrom;

	// if file load is successful, update current preset name and call setStateInformation
	// with loaded data
	if (irToLoad.loadFileAsData(blockToLoadFrom))
	{
		audioProcessor->setStateInformation(blockToLoadFrom.getData(), static_cast<int>(blockToLoadFrom.getSize()));

		currentImpulseResponseName = irToLoad.getFileNameWithoutExtension();

		currentImpulseResponse = juce::File(irToLoad.getFullPathName());
	}
}

void ImpulseResponseManager::clearImpulseResponse()
{
	currentImpulseResponseName = "No IR Loaded";
	currentImpulseResponse = noIRLoaded;
	
	ImpulseResponseLoaded = false;
}

const juce::String ImpulseResponseManager::getImpulseResponseName(int inIrIndex)
{
	// return name of IR at given index
	return impulseResponseList[inIrIndex].getFileNameWithoutExtension();
}

const juce::String ImpulseResponseManager::getCurrentImpulseResponseName()
{
	// return name of currently loaded preset
	return currentImpulseResponseName;
}

void ImpulseResponseManager::setCurrentImpulseResponseName(juce::String irName)
{
	currentImpulseResponseName = irName;
}

const juce::String ImpulseResponseManager::getCurrentImpulseResponseDirectory()
{
	// return path of root preset directory as a string
	return impulseResponseDirectory.getFullPathName();
}

juce::File ImpulseResponseManager::getCurrentImpulseResponse()
{
	return currentImpulseResponse;
}

bool ImpulseResponseManager::isImpulseResponseLoaded()
{
	return ImpulseResponseLoaded;
}

int ImpulseResponseManager::getNumberOfImpulseResponses()
{
	// return size of IR list
	return impulseResponseList.size();
}

void ImpulseResponseManager::updateImpulseResponseList()
{
	// clear preset list
	impulseResponseList.clear();

	// create a RangedDirectoryIterator
	auto directoryIterator = juce::RangedDirectoryIterator(juce::File(impulseResponseDirectory),
		false,
		"*.wav",
		juce::File::TypesOfFileToFind::findFiles);

	// iterate through the directory, adding the file names to preset list
	for (auto entry : directoryIterator)
		impulseResponseList.add(entry.getFile());
}
