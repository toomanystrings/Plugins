/*
  ==============================================================================

    Runes.h
    Created: 2 Dec 2020 5:52:53pm
    Author:  Olorin

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class Runes  : public juce::Component
{
public:
    Runes()
    {
        // In your constructor, you should add any child components, and
        // initialise any special settings that your component needs.
		setSize(20, 20);

    }

    ~Runes() override
    {
    }

    void paint (juce::Graphics& g) override
    {
        /* This demo code just fills the component's background and
           draws some placeholder text to get you started.

           You should replace everything in this method with your own
           drawing code..
        */

		g.fillAll(juce::Colour::fromRGB(0.0f, 0.0f, 0.0f).withAlpha(0.0f));   // clear the background

		// Basic measurements
		const auto bounds = getBounds();
		const auto width = bounds.getWidth();
		const auto height = bounds.getHeight();

		const auto centreX = width / 2;
		const auto centreY = height / 2;

		const auto tX = width / 3;
		const auto tY = height / 3;

		const auto qX = width / 4;
		const auto qY = height / 4;

        switch (runeIndex)
        {
		case 0:
			rune.startNewSubPath(qX * 3, height);
			rune.lineTo(qX, qY);
			rune.lineTo(centreX, 0);
			rune.lineTo(qX * 3, qY);
			rune.lineTo(qX, height);
			break;
        	
		case 1:
			rune.startNewSubPath(tX, 0);
			rune.lineTo(tX, height);
			rune.closeSubPath();
        	
			rune.startNewSubPath(tX, qY);
			rune.lineTo(qX * 3, 0);
			rune.closeSubPath();
        	
			rune.startNewSubPath(tX, centreY);
			rune.lineTo(qX * 3, qY);
			break;
        	
		case 2:
			rune.startNewSubPath(qX, height);
			rune.lineTo(qX, 0);
			rune.lineTo(qX * 3, tY);
			rune.lineTo(qX * 3, height);
			break;
        	
		case 3:
			rune.startNewSubPath(tX, 0);
			rune.lineTo(tX, height);
			rune.closeSubPath();
        	
			rune.startNewSubPath(tX, qY);
			rune.lineTo(tX * 2, centreY);
			rune.lineTo(tX, qY * 3);
			break;
        	
		case 4:
			rune.startNewSubPath(tX, 0);
			rune.lineTo(tX, height);
			rune.closeSubPath();
        	
			rune.startNewSubPath(tX, 0);
			rune.lineTo(qX * 3, qY);
			rune.closeSubPath();
        	
			rune.startNewSubPath(tX, qY);
			rune.lineTo(qX * 3, centreY);
			break;
        	
		case 5:
			rune.startNewSubPath(qX, height);
			rune.lineTo(qX, 0);
			rune.lineTo(qX * 3, qY);
			rune.lineTo(qX, centreY);
			rune.lineTo(qX * 3, height);
			break;
        	
		case 6:
			rune.startNewSubPath(qX * 3, qY);
			rune.lineTo(qX, centreY);
			rune.lineTo(qX * 3, qY * 3);
			break;
        	
		case 7:
			rune.startNewSubPath(qX, 0);
			rune.lineTo(qX * 3, height);
			rune.closeSubPath();
        	
			rune.startNewSubPath(qX * 3, 0);
			rune.lineTo(qX, height);
			break;
        	
		case 8:
			rune.startNewSubPath(qX, height);
			rune.lineTo(qX, 0);
			rune.lineTo(qX * 3, qY);
			rune.lineTo(qX, centreY);
			break;
        	
		case 9:
			rune.startNewSubPath(qX, 0);
			rune.lineTo(qX, height);
			rune.closeSubPath();

			rune.startNewSubPath(qX * 3, 0);
			rune.lineTo(qX * 3, height);
			rune.closeSubPath();

			rune.startNewSubPath(qX, qY);
			rune.lineTo(qX * 3, qY * 3);
			break;
        	
		case 10:
			rune.startNewSubPath(centreX, 0);
			rune.lineTo(centreX, height);
			rune.closeSubPath();

			rune.startNewSubPath(qX, qY);
			rune.lineTo(qX * 3, qY * 3);
			break;
        	
		case 11:
			rune.startNewSubPath(centreX, 0);
			rune.lineTo(centreX, height);
			break;
        	
		case 12:
			break;
        	
		case 13:
			break;
        	
		case 14:
			break;
        	
		case 15:
			break;
        	
		case 16:
			break;
        	
		case 17:
			break;
        	
		case 18:
			rune.startNewSubPath(centreX, height);
			rune.lineTo(centreX, 0);
			rune.lineTo(qX * 3, tY);

			rune.startNewSubPath(centreX, tY);
			rune.lineTo(qX * 3, centreY);
			rune.lineTo(centreX, qY * 3);
			rune.lineTo(qX, centreY);
			rune.closeSubPath();

			break;
        }
		
		

		auto pathWidth = 2.0f;
		g.setColour(juce::Colour::fromRGB(74, 0, 0).withAlpha(0.7f));
		g.strokePath(rune, juce::PathStrokeType(pathWidth, juce::PathStrokeType::JointStyle::beveled));
    	
    }

    void resized() override
    {
        // This method is where you should set the bounds of any child
        // components that your component contains..

    }

	void setRune(int runeIndex) { this->runeIndex = runeIndex; }

	int getRune() { return runeIndex; }

private:
	juce::Path rune;

	int runeIndex = 18;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Runes)
};
