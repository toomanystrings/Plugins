/*
  ==============================================================================

    PresetManager.cpp
    Created: 28 Aug 2020 5:28:22pm
    Author:  Jose Diaz

  ==============================================================================
*/

#include "PresetManager.h"

PresetManager::PresetManager (juce::AudioProcessor* p) : audioProcessor(p)
{
    // create the presetDirectory root file
	presetDirectory = juce::File(juce::File::getSpecialLocation(juce::File::userDocumentsDirectory).getFullPathName()
									+ directorySeperator /*"\\"*/ + "Tote Bag"
									+ directorySeperator /*"\\"*/ + audioProcessor->getName()
									+ directorySeperator /*"\\"*/ + "Presets");
	
    // check if that directory exists, if not, we need to do something about it
    // here we simply create the directory, but this is not ideal—your plugin installer should
    // create the preset directory
    // perhaps fall back to a directory that is guaranteed to exist
	if (!presetDirectory.exists())
		presetDirectory.createDirectory();

    // update the locally held list of presets
	updatePresetList();
}



void PresetManager::createNewPreset()
{
    // here you would iterate through all of the APVTS parameters and set them
    // to their default values, then set currentPresetName to "Untitled" or some init. state
}

void PresetManager::savePreset (juce::File presetToSave)
{
    // check if the file passed in actually exists.
    // if it does, delete it. if not, create it
	if (!presetToSave.exists())
		presetToSave.create();
	else
		presetToSave.deleteFile();
	
    
    // update preset name for GUI display using file name
	currentPresetName = presetToSave.getFileNameWithoutExtension();
	//currentPresetName.removeCharacters(juce::StringRef("*"));

    // allocate memory on the stack using MemoryBlock, and fill it with our preset data
    // using get state information
	juce::MemoryBlock blockToSaveTo;

	audioProcessor->getStateInformation(blockToSaveTo);
        
    // write the preset data to the file
	presetToSave.appendData(blockToSaveTo.getData(), static_cast<int>(blockToSaveTo.getSize()));
    
    // update the preset list
	updatePresetList();
}

void PresetManager::loadPreset (juce::File presetToLoad)
{
    // create a MemoryBlock onto which we load the preset data
	juce::MemoryBlock blockToLoadFrom;
    
    // if file load is successful, update current preset name and call setStateInformation
    // with loaded data
	if(presetToLoad.loadFileAsData(blockToLoadFrom))
	{
		audioProcessor->setStateInformation(blockToLoadFrom.getData(), static_cast<int>(blockToLoadFrom.getSize()));
		currentPresetName = presetToLoad.getFileNameWithoutExtension();
	}
}


const juce::String PresetManager::getPresetName (int inPresetIndex)
{
    // return name of preset at given index
	return presetList[inPresetIndex].getFileNameWithoutExtension();
}

const juce::String PresetManager::getCurrentPresetName()
{
    // return name of currently loaded preset
	return currentPresetName;
}

int PresetManager::getNumberOfPresets()
{
    // return size of preset list
	return presetList.size();
}

const juce::String PresetManager::getCurrentPresetDirectory()
{
   // return path of root preset directory as a string
	return presetDirectory.getFullPathName();
}

void PresetManager::setCurrentPresetName (juce::String newPresetName)
{
    // set the current preset name
	currentPresetName = newPresetName;
}

//======================================================================================================

void PresetManager::updatePresetList()
{
    // clear preset list
	presetList.clear();
        
    // create a RangedDirectoryIterator
	auto directoryIterator = juce::RangedDirectoryIterator(juce::File(presetDirectory), 
												 false, 
												 presetFileExtensionWildcard,
												 juce::File::TypesOfFileToFind::findFiles);
    
    // iterate through the directory, adding the file names to preset list
	for (auto entry : directoryIterator)
		presetList.add(entry.getFile());
}
