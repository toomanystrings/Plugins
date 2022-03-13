/*
  ==============================================================================

    SynthLookAndFeel.h
    Created: 23 Jun 2019 8:01:19pm
    Author:  Conor

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../Utilities.h"


class SynthLookAndFeel : public juce::LookAndFeel_V4
{
	void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
						  const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider) override
	{
		auto radius = juce::jmin(width / 2, height / 2) - 5.0f;
		auto centreX = x + width / 2;
		auto centreY = y + height / 2;
		auto rx = centreX - radius;
		auto ry = centreY - radius;
		auto rw = radius * 2.0f;
		auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

		juce::Colour sliderFill(juce::Colours::black);
		juce::Colour sliderEdge(juce::Colour::fromRGB(74, 0, 0));
		juce::Colour textBoxEdge(juce::Colour::fromRGBA(0,0,0,0));

		// Fill
		g.setColour(sliderFill);
		g.fillEllipse(rx, ry, rw, rw);

		// Outline
		g.setColour(sliderEdge);
		g.drawEllipse(rx, ry, rw, rw, 2.0f);

		// Dial tick
		juce::Path tick;
		auto tickLength = radius / 3.0f;
		auto tickThickness = 3.0f;
		
		tick.addRectangle(-tickThickness * 0.5f, -radius, tickThickness, tickLength);
		tick.applyTransform(juce::AffineTransform::rotation(angle).translated(centreX, centreY));

		// Draw tick
		g.fillPath(tick);

		slider.setColour(0x1001700, textBoxEdge);
		slider.setColour(0x1001400, sliderFill);
	}

	juce::Font getLabelFont(juce::Label &label) override
	{
		return DVFonts::getBoldNorse(15);
	}

	void drawComboBox(juce::Graphics &g, int width, int height, bool isButtonDown, int buttonX, int buttonY, int buttonW, int buttonH, juce::ComboBox &comboBox) override
	{
		const int cutoff = static_cast<int>(juce::jmin(width, height) * 0.33f);

		juce::Colour background(juce::Colours::black);
		//juce::Colour outline(juce::Colour::fromRGB(74, 0, 0));

		juce::Path p;

		if (!comboBox.isPopupActive())
		{
			p.startNewSubPath(0, 0);
			p.lineTo(width - cutoff, 0);
			p.lineTo(width, cutoff);
			p.lineTo(width, height);
			p.lineTo(cutoff, height);
			p.lineTo(0, height - cutoff);
			p.closeSubPath();

			g.setColour(background);
			g.fillPath(p);
		}
		else
		{
			p.startNewSubPath(0, 0);
			p.lineTo(width - cutoff, 0);
			p.lineTo(width, cutoff);
			p.lineTo(width, height);
			p.lineTo(0, height);
			p.closeSubPath();

			g.setColour(background);
			g.fillPath(p);
		}
		
		comboBox.setColour(juce::ComboBox::textColourId, juce::Colours::whitesmoke);
		comboBox.setColour(juce::PopupMenu::ColourIds::backgroundColourId, juce::Colours::black.withAlpha(0.f));
		//g.setFont(norse);
		//g.drawText(comboBox.getText(), 10, 0, width, height, juce::Justification::left);
	}

	void drawPopupMenuBackground(juce::Graphics& g, int width, int height) override
	{
		juce::Path p;

		p.addRectangle(0, 0, width, height);

		g.fillAll(juce::Colours::black);

		g.setColour(juce::Colour::fromRGB(74, 0, 0));
		g.strokePath(p, juce::PathStrokeType(2));
	}

	void drawPopupMenuItem(juce::Graphics& g, const juce::Rectangle< int >& area, bool isSeparator, bool isActive, bool isHighlighted,
	                       bool isTicked, bool hasSubMenu, const juce::String& text, const juce::String& shortcutKeyText, const juce::Drawable* icon, const juce::Colour* textColour) override
	{
		const auto height = area.getHeight();
		const auto width = area.getWidth();

		const int cutoff = static_cast<int>(juce::jmin(width, height) * 0.33f);

		juce::Path p;

		p.startNewSubPath(2, 0);
		p.lineTo(width - cutoff, 0);
		p.lineTo(width - 2, cutoff);
		p.lineTo(width - 2, height);
		p.lineTo(cutoff, height);
		p.lineTo(2, height - cutoff);
		p.closeSubPath();

		const juce::Colour background = juce::Colours::black;
		const juce::Colour outline(juce::Colour::fromRGB(74, 0, 0));
		const juce::Colour highlight(juce::Colour::fromRGB(74, 0, 0).withAlpha(0.5f));
		
		if(isHighlighted)
		{
			if(isTicked)
			{
				g.setColour(highlight);
				g.fillPath(p);

				g.setColour(outline);
				g.strokePath(p, juce::PathStrokeType(1, juce::PathStrokeType::JointStyle::mitered));
			}
			else
			{
				g.setColour(highlight);
				g.fillPath(p);
			}
		}
		else
		{
			if (isTicked)
			{
				g.setColour(background);
				g.fillPath(p);

				g.setColour(outline);
				g.strokePath(p, juce::PathStrokeType(1, juce::PathStrokeType::JointStyle::mitered));
			}
			else
			{
				g.setColour(background);
				g.fillPath(p);
			}
		}
		

		g.setColour(juce::Colours::whitesmoke);
		g.setFont(DVFonts::getBoldNorse(15));
		
		g.drawText(text, 10, 0, width - 10, height, juce::Justification::centredLeft);
	}


	juce::Font getComboBoxFont(juce::ComboBox& comboBox) override
	{
		return getCommonMenuFont();
	}

	juce::Font getPopupMenuFont() override
	{
		return getCommonMenuFont();
	}

private:
	juce::Font getCommonMenuFont()
	{
		return DVFonts::getBoldNorse(15);
	}

	//Utilities utils;
	//juce::Font norse = utils.getNorseBoldFont();
};
