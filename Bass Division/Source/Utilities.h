/*
  ==============================================================================

    Utilities.h
    Created: 19 Nov 2020 2:49:44pm
    Author:  Olorin

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#if JUCE_WINDOWS
static const juce::String directorySeperator = "\\";

#elif JUCE_MAC
static const juce::String directorySeperator = "/";
#endif

namespace DVFonts
{
	static const juce::Font& getBoldNorse(float height)
	{
		//static juce::Font norse(juce::Font(juce::Typeface::createSystemTypefaceFor(BinaryData::NorseKaWl_otf, BinaryData::NorseKaWl_otfSize)));
        static juce::Font norse(juce::Font(juce::Typeface::createSystemTypefaceFor(BinaryData::Norsebold_otf, BinaryData::Norsebold_otfSize)));
		norse.setHeight(height);
		return norse;// .boldened();
	}

	static juce::Font norseBold(juce::Font(juce::Typeface::createSystemTypefaceFor(BinaryData::Norsebold_otf, BinaryData::Norsebold_otfSize)));
	//static juce::Font norse(juce::Font(juce::Typeface::createSystemTypefaceFor(BinaryData::Norse_otf, BinaryData::Norse_otfSize)));
}

class Utilities
{
public:
	Utilities() {}

	

private:


	
};
