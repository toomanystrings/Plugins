/*
  ==============================================================================

    NormalKnobLAF.h
    Created: 20 Feb 2021 2:33:54pm
    Author:  Olorin

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Utilities.h"

class NormalKnobLAF : public juce::LookAndFeel_V4
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

		juce::ColourGradient haloFill(juce::Colours::whitesmoke, rxHalo, ryHalo, juce::Colours::darkred, rxHalo + rwHalo, ryHalo, false);
		
		juce::Path filledArc;
		//g.setColour(haloFill);
		g.setGradientFill(haloFill);
		filledArc.addPieSegment(rxHalo, ryHalo, rwHalo, rwHalo, rotaryStartAngle, angle, haloThickness);
		g.fillPath(filledArc);

		g.setColour(juce::Colours::black);
		g.strokePath(haloBackground, juce::PathStrokeType(outerEdge));

		//juce::Colour sliderFill(juce::Colours::black);
		//juce::Colour sliderEdge(juce::Colours::grey);
		//juce::Colour textBoxEdge(juce::Colour::fromRGBA(0, 0, 0, 0));

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
		//norse.setBold(true);
		return DVFonts::getBoldNorse(15);
	}

	void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const juce::Slider::SliderStyle style, juce::Slider& slider) override
	{
		
		
		drawLinearSliderBackground(g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, style, slider);

		
		drawLinearSliderThumb(g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, style, slider);

		slider.setColour(0x1001700, textBoxEdge);
		slider.setColour(0x1001400, sliderFill);
	}

	void drawLinearSliderBackground(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const juce::Slider::SliderStyle style, juce::Slider& slider) override
	{
		auto bounds = juce::Rectangle<int>(x, y, width, height);

		auto centreX = width / 2;
		auto centreY = height / 2;

		auto railW = width / 5.5;
		auto railFromCentre = railW / 2;

		juce::Path sliderLine;

		g.setColour(juce::Colours::grey);

		auto textBoxHeight = slider.getTextBoxHeight();




		g.fillRoundedRectangle(centreX - railFromCentre, y, railW, height, 2);

		const juce::ColourGradient gradient(juce::Colours::darkred, centreX, 0, juce::Colours::whitesmoke, centreX, height,false);
		g.setGradientFill(gradient);

		g.fillRoundedRectangle(centreX - railFromCentre, sliderPos, railW, height, 2);

		g.setColour(juce::Colours::black);

		g.drawRoundedRectangle(centreX - railFromCentre, y, railW, height, 2, 1);
	}

	void drawLinearSliderThumb(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const juce::Slider::SliderStyle style, juce::Slider& slider) override
	{
		auto bounds = juce::Rectangle<int>(x, y, width, height);

		auto centreX = width / 2;
		

		auto thumbW = width / 2.7;
		auto thumbFromCentre = thumbW / 2;


		juce::Rectangle<float> thumbArea(centreX - thumbFromCentre, sliderPos - (thumbW * 1.5 / 2), thumbW, thumbW * 1.5);
		
		g.setColour(juce::Colours::black);
		g.fillRoundedRectangle(thumbArea, 4);

		//thumbArea.reduced(0.3);

		g.setColour(juce::Colours::grey);

		g.drawRoundedRectangle(thumbArea.reduced(1.4), 4, 1);

		g.drawLine(centreX - thumbFromCentre * 0.5, sliderPos, centreX + thumbFromCentre * 0.5, sliderPos);

		
	}



private:
	//Utilities utils;
	//juce::Font norse = utils.getNorseBoldFont();

	juce::Colour sliderFill = juce::Colours::black;
	juce::Colour sliderEdge = juce::Colours::grey;
	juce::Colour textBoxEdge = juce::Colour::fromRGBA(0, 0, 0, 0);
	
};

