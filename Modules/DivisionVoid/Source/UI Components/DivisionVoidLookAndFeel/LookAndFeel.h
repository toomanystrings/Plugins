//
// Created by Conor on 11/04/2022.
//
#pragma once

//#include "juceheader.h"

namespace DivisionVoid
{
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
            auto centreX = x + (width / 2);
            auto centreY = y + (height / 2) + width * 0.045f;
            auto rx = centreX - radius;
            auto ry = centreY - radius;
            auto rw = radius * 2;

            // Control how wide the range of movement is of the slider
            float angleStartOffset = 0.3f;

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
            return fonts.getFont(DivisionVoidFonts::FontType::bold, 15.0f);
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

        DivisionVoidFonts fonts;

        juce::Colour sliderFill = juce::Colours::black;
        juce::Colour sliderEdge = juce::Colours::grey;
        juce::Colour textBoxEdge = juce::Colour::fromRGBA(0, 0, 0, 0);
    };

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
            return fonts.getFont(DivisionVoidFonts::FontType::bold, 15.0f);
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

    private:

        DivisionVoidFonts fonts;

        juce::Colour sliderFill = juce::Colours::black;
        juce::Colour sliderEdge = juce::Colours::grey;
        juce::Colour textBoxEdge = juce::Colour::fromRGBA(0, 0, 0, 0);

        const float scaleFactor = 0.33f;
    };

    class ComboBoxLookAndFeel : public juce::LookAndFeel_V4
    {
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
            g.setFont(fonts.getFont(DivisionVoidFonts::FontType::bold, 15.0f));

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
            return fonts.getFont(DivisionVoidFonts::FontType::bold, 15.0f);
        }

        DivisionVoidFonts fonts;
    };


}
