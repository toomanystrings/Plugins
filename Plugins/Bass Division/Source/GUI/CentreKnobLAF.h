/*
  ==============================================================================

    CentreKnobLAF.h
    Created: 20 Feb 2021 2:33:29pm
    Author:  Olorin

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Utilities.h"

class CentreKnobLAF : public juce::LookAndFeel_V4
{
	void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
		float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override
	{
		// Determine the amount of area that the whole knob will take up
		auto sizeCoefficient = 0.75f;

		// General maths stuff for determining the area and angles
		auto diameter = juce::jmin(width, height);
		auto radius = diameter / 2 * sizeCoefficient;
		auto centreX = x + width / 2;
		auto centreY = (y + height / 2) + width * 0.045f;
		auto rx = centreX - radius;
		auto ry = centreY - radius;
		auto rw = radius * 2;

		// Control how wide the range of movement is of the slider
		float angleStartOffset = 0.3;

		// Leave these alone, they're working out important angle-y business.
		rotaryStartAngle = rotaryStartAngle + angleStartOffset;
		rotaryEndAngle = rotaryEndAngle - angleStartOffset;
		auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

		// Order now becomes important. We need to draw the halo background, then the bit that is filled, followed by the knob itself

		/*--- DRAW THE HALO---*/
		auto haloSizeRatio = 1.4f;

		auto haloRadius = radius * haloSizeRatio;
		auto rxHalo = centreX - haloRadius;
		auto ryHalo = centreY - haloRadius;
		auto rwHalo = haloRadius * 2.0f;
		const auto haloThickness = 0.8f;
		const auto outerEdge = radius * 0.03f;

		juce::Path haloBackground;
		g.setColour(juce::Colours::whitesmoke);
		haloBackground.addPieSegment(rxHalo, ryHalo, rwHalo, rwHalo, rotaryStartAngle, rotaryEndAngle, haloThickness);
		g.fillPath(haloBackground);

		juce::Path filledArc;
		//g.setColour(juce::Colours::green);

		auto centreNotch = 6.28319f;

		if (angle > centreNotch)
		{
			juce::ColourGradient haloFill(juce::Colours::whitesmoke, centreX, ryHalo, juce::Colours::darkred, rxHalo + rwHalo, ryHalo + rwHalo, false);
			g.setGradientFill(haloFill);
			filledArc.addPieSegment(rxHalo, ryHalo, rwHalo, rwHalo, centreNotch, angle, haloThickness);
			
		}
		else
		{
			juce::ColourGradient haloFill(juce::Colours::darkred, rxHalo, ryHalo + rwHalo, juce::Colours::whitesmoke, centreX, ryHalo, false);
			g.setGradientFill(haloFill);
			filledArc.addPieSegment(rxHalo, ryHalo, rwHalo, rwHalo, angle, centreNotch, haloThickness);
		}
			//filledArc.addPieSegment(rxHalo, ryHalo, rwHalo, rwHalo, angle, centreNotch, haloThickness);
		g.fillPath(filledArc);

		g.setColour(juce::Colours::black);
		g.strokePath(haloBackground, juce::PathStrokeType(outerEdge));

		juce::Colour sliderFill(juce::Colours::black);
		juce::Colour sliderEdge(juce::Colours::grey);
		juce::Colour textBoxEdge(juce::Colour::fromRGBA(0, 0, 0, 0));

		//juce::Colour::fromRGB(90, 0, 0)

		// :::: DRAW MAIN CIRCLE PART ::::
		// Make some definitions for paths.
		juce::Path innerCircle;
		juce::Path hiddenInnerBorder;
		juce::Path borderCircle;
		juce::Path tick;

		// Some helpful variables you can use to modify the sizes of different aspects of the slider.
		float innerCircleSize = sizeCoefficient * 0.92f;
		float hiddenInnerBorderSize = sizeCoefficient * 0.97f;
		float borderCircleSize = sizeCoefficient * 1.0f;
		float innerCircleDistanceFromCentre = 7 * sizeCoefficient;
		float innerCircleDiameter = 35 * sizeCoefficient;

		// MAIN BORDER (In the OG design this was the black/dark grey bit)
		g.setColour(sliderFill);
		borderCircle.addEllipse(centreX - diameter * (borderCircleSize / 2), centreY - diameter * (borderCircleSize / 2),
			diameter * borderCircleSize, diameter * borderCircleSize);
		g.fillPath(borderCircle);

		// SUBTLE INNER BORER
		g.setColour(sliderEdge);
		hiddenInnerBorder.addEllipse(centreX - diameter * (hiddenInnerBorderSize / 2), centreY - diameter * (hiddenInnerBorderSize / 2),
			diameter * hiddenInnerBorderSize, diameter * hiddenInnerBorderSize);
		g.fillPath(hiddenInnerBorder);

		// MAIN INNER CIRCLE OF THE KNOB.
		g.setColour(sliderFill);
		innerCircle.addEllipse(centreX - diameter * (innerCircleSize / 2), centreY - diameter * (innerCircleSize / 2),
			diameter * innerCircleSize, diameter * innerCircleSize);
		g.fillPath(innerCircle);

		// TICK 
		float tickOuterOffset = 0.15 * radius;
		float tickWidth = radius * 0.07;
		float tickLength = 0.32 * radius;
		//float tickRoundness = 3.5;
		g.setColour(sliderEdge);
		tick.addRectangle(0 - tickWidth / 2, -radius + tickOuterOffset, tickWidth, tickLength);
		g.fillPath(tick, juce::AffineTransform::rotation(angle).translated(centreX, centreY));

		slider.setColour(0x1001700, textBoxEdge);
		slider.setColour(0x1001400, sliderFill);

	}

	juce::Font getLabelFont(juce::Label& label) override
	{
		//juce::Font test()
		return DVFonts::getBoldNorse(15);
	}

	void drawTabButton(juce::TabBarButton& buttonBar, juce::Graphics& g, bool isMouseOver, bool isMouseDown) override
	{
		auto width = buttonBar.getWidth();
		auto height = buttonBar.getHeight();

		int cutoff = static_cast<int>(std::min(width, height) * scaleFactor);

		juce::Path p;
		//p.addRoundedRectangle(0, 0, width, height, CORNER_RADIUS, CORNER_RADIUS, false, !(isConnectedOnTop() || isConnectedOnRight()), !(isConnectedOnBottom() || isConnectedOnLeft()), false);
		p.startNewSubPath(width, 0);
		p.lineTo(width, height - cutoff);
		p.lineTo(width - cutoff, height);
		p.lineTo(0, height);
		p.lineTo(0, cutoff);
		p.lineTo(cutoff, 0);
		p.closeSubPath();

		g.setColour(juce::Colours::black);
		g.fillPath(p);

		g.setColour(juce::Colours::grey);
		//g.setFont(norse);
		g.drawMultiLineText(buttonBar.getButtonText(), width / 2 - 5, height / 2, 10, juce::Justification::centred);
	}

	/*void drawTabButtonText(juce::TabBarButton& buttonBar, juce::Graphics& g, bool isMouseOver, bool isMouseDown) override
	{
		g.setColour(juce::Colours::grey);
		g.setFont(norse);
		g.drawMultiLineText(buttonBar.getButtonText(), 0, 0, 10, juce::Justification::centred);
	}*/
	
private:
	//Utilities utils;
	//juce::Font norse = utils.getNorseBoldFont();

	juce::Colour sliderFill = juce::Colours::black;
	juce::Colour sliderEdge = juce::Colours::grey;
	juce::Colour textBoxEdge = juce::Colour::fromRGBA(0, 0, 0, 0);

	const float scaleFactor = 0.33f;

};
