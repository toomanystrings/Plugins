//
// Created by Conor Foran on 27/02/2023.
//

#pragma once

namespace DivisionVoid
{
    class DoubleStackLAF : public juce::LookAndFeel_V4
    {
        void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
                              float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override
        {
            // Determine the amount of area that the whole knob will take up
            auto sizeCoefficient = 1.0f;

            // General maths stuff for determining the area and angles
            auto diameter = juce::jmin(width, height);
            auto radius = diameter / 2 * sizeCoefficient;
            auto centreX = x + width / 2;
            auto centreY = y + height / 2;

            // Control how wide the range of movement is of the slider
            float angleStartOffset = 0.0f;

            // Leave these alone, they're working out important angle-y business.
            rotaryStartAngle = rotaryStartAngle + angleStartOffset;
            rotaryEndAngle = rotaryEndAngle - angleStartOffset;
            auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

            // :::: DRAW MAIN CIRCLE PART ::::
            // Make some definitions for paths.
            juce::Path innerCircle;
            juce::Path hiddenInnerBorder;
            juce::Path borderCircle;
            juce::Path tick;

            // Some helpful variables you can use to modify the sizes of different aspects of the slider.
            float innerCircleSize = sizeCoefficient * 0.97f;
            float borderCircleSize = sizeCoefficient * 1.0f;

            // BORDER
            // This is the colour around the outside. This works differently in the other knob LAF in this module.
            // I have changed this here as the inner and outer borders/fill were the same colour, and looks to be
            // overlapping when selecting the outer slider but you actually get the inner slider.
            g.setColour(sliderEdge);
            borderCircle.addEllipse(centreX - diameter * (borderCircleSize / 2), centreY - diameter * (borderCircleSize / 2),
                                    diameter * borderCircleSize, diameter * borderCircleSize);
            g.fillPath(borderCircle);

            // INNER CIRCLE
            g.setColour(sliderFill);
            innerCircle.addEllipse(centreX - diameter * (innerCircleSize / 2), centreY - diameter * (innerCircleSize / 2),
                                   diameter * innerCircleSize, diameter * innerCircleSize);
            g.fillPath(innerCircle);

            // TICK
            float tickOuterOffset = radius * 0.01;
            float tickWidth = radius * 0.05;
            float tickLength = radius * 0.2;
            g.setColour(sliderEdge);
            tick.addRectangle(0 - tickWidth / 2, -radius + tickOuterOffset, tickWidth, tickLength);
            g.fillPath(tick, juce::AffineTransform::rotation(angle).translated(centreX, centreY));

            slider.setColour(0x1001700, textBoxEdge);
            slider.setColour(0x1001400, sliderFill);
        }

        DivisionVoidFonts fonts;

        juce::Colour sliderFill = juce::Colours::black;
        juce::Colour sliderEdge = juce::Colours::grey;
        juce::Colour textBoxEdge = juce::Colour::fromRGBA(0, 0, 0, 0);
    };

    class InnerSlider : public juce::Slider
    {
    public:
        bool hitTest(int x, int y) override
        {
            auto pos = juce::Point<int>(x, y);
            return centre.getDistanceFrom(pos) < radius;
        }

        void resized() override
        {
            centre = juce::Point<int>(getWidth() / 2, getHeight() / 2);
            radius = std::min(getWidth(), getHeight()) / 2;
            juce::Slider::resized();
        }

        bool getIsCursorInInnerSlider() { return isCursorInInnerSlider; }

        void mouseEnter(const juce::MouseEvent &e)
        {
            isCursorInInnerSlider = true;
        }

        void mouseExit(const juce::MouseEvent &e)
        {
            isCursorInInnerSlider = false;
        }

    private:
        juce::Point<int> centre;
        int radius;

        bool isCursorInInnerSlider = false;
    };

    // This is a straight inhereitance from juce::Slider. This has been done purely to make objects in the double
    // stack class more readable. While the actual names you assign to the objects themselves would likely do the
    // job, for consistencies sake, I have chosen to do it this way. Nothing has been overridden as yet.
    class OuterSlider : public juce::Slider
    {

    };

    // An object containing two sliders. One inner and one outer. These sliders are called individually for attachment
    // purposes by using the methods getInnerSlider() and getOuterSlider(). This returns references to the individual
    // objects
    class DoubleStackKnob : public juce::Component, private juce::Timer
    {
    public:
        DoubleStackKnob()
        {
            initialiseSliders(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);

            outerSlider.setLookAndFeel(&laf);
            innerSlider.setLookAndFeel(&laf);

            addAndMakeVisible(outerSlider);
            addAndMakeVisible(innerSlider);

            startTimerHz(15);
        }

        DoubleStackKnob(float outerProportion, float innerProportion, bool shouldDrawPips) :
        outerProportionalDiameter (outerProportion),
        innerProportionalDiameter (innerProportion),
        drawPips (shouldDrawPips)
        {
            initialiseSliders(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);

            outerSlider.setLookAndFeel(&laf);
            innerSlider.setLookAndFeel(&laf);

            addAndMakeVisible(outerSlider);
            addAndMakeVisible(innerSlider);

            startTimerHz(15);
        }

        ~DoubleStackKnob() { stopTimer(); }

        void paint (juce::Graphics &g) override
        {
            auto bounds = getLocalBounds();

            // We will need a few bits of information, such as the start and end angle of the knob(s) in
            // radians.
            auto rotaryParams = outerSlider.getRotaryParameters();
            auto startAngleRad = rotaryParams.startAngleRadians;
            auto endAngleRad = rotaryParams.endAngleRadians;

            // We will need the centre X and Y
            juce::Point<int> centre(getWidth() / 2, getHeight() / 2); // Overkill
            auto centreX = centre.getX();
            auto centreY = centre.getY();

            // We will also need the shortest side!
            auto shortestSide = juce::jmin(bounds.getWidth(), bounds.getHeight());

            // Radii values used for tick drawing
            auto tickRadius = shortestSide * 0.88f / 2;
            auto textRadius = shortestSide * 0.95f / 2;

            if (drawPips)
            {
                // Before we draw the default, we should check to see if the inner section is "in focus"
                if (!mouseOverInnerSlider)
                {
                    // This is default, and thus the outer slider values.
                    auto numTicks = outerValues.size();

                    for (auto i = 0; i < numTicks; ++i)
                    {
                        auto angle = startAngleRad + ((endAngleRad - startAngleRad) * i / (numTicks - 1));
                        auto text = juce::String(outerValues[i]);

                        drawTick(tickRadius, angle, centreX, centreY, g, juce::Colours::black);
                        drawText(textRadius, angle, centreX, centreY, g, juce::Colours::black, text);
                    }
                }
                else
                {
                    auto numTicks = innerValues.size();

                    for (auto i = 0; i < numTicks; ++i)
                    {
                        auto angle = startAngleRad + ((endAngleRad - startAngleRad) * i / (numTicks - 1));
                        auto text = juce::String(innerValues[i]);

                        drawTick(tickRadius, angle, centreX, centreY, g, juce::Colours::black);
                        drawText(textRadius, angle, centreX, centreY, g, juce::Colours::black, text);
                    }
                }
            }
        }

        void resized() override
        {
            auto shortestSide = juce::jmin(getWidth(), getHeight());

            // Determines the full width of the sliders
            auto outerDiameter = shortestSide * outerProportionalDiameter;
            auto innerDiameter = shortestSide * innerProportionalDiameter;

            // Determines the x and y co-ords of the sliders
            auto outerWidthOffset = (getWidth() - outerDiameter) / 2;
            auto outerHeightOffset = (getHeight() - outerDiameter) / 2;
            auto innerWidthOffset = (getWidth() - innerDiameter) / 2;
            auto innerHeightOffset = (getHeight() - innerDiameter) / 2;

            outerSlider.setBounds(outerWidthOffset, outerHeightOffset, outerDiameter, outerDiameter);
            innerSlider.setBounds(innerWidthOffset, innerHeightOffset, innerDiameter, innerDiameter);
        }

        // Returns inner slider for use with value tree states.
        InnerSlider &getInnerSlider() { return innerSlider; }
        // Returns outer slider for use with value tree states.
        OuterSlider &getOuterSlider() { return outerSlider; }

        // proportion needs to be within a range of 0 and 1
        void setOuterProportionalDiameter(float proportion)
        {
            jassert(proportion > 0.0f && proportion <= 1.0f);
            this -> outerProportionalDiameter = proportion;
        }
        float getOuterProportionalDiameter() const { return outerProportionalDiameter; }

        // proportion needs to be within a range of 0 and 1
        void setInnerProportionalDiameter(float proportion)
        {
            jassert(proportion > 0.0f && proportion <= 1.0f);
            this ->innerProportionalDiameter = proportion;
        }
        float getInnerProportionalDiameter() const { return innerProportionalDiameter; }

        void timerCallback() override
        {
            if (mouseOverInnerSlider != innerSlider.getIsCursorInInnerSlider() && drawPips)
            {
                mouseOverInnerSlider = innerSlider.getIsCursorInInnerSlider();
                repaint();
            }
        }

        // Takes a std::vector<float> of values to use when drawing pips. This then determines how many
        // ticks are drawn
        void setInnerTickValues(std::vector<float> values)
        {
            innerValues.clear();
            innerValues = values;
        }

        // Takes a std::vector<float> of values to use when drawing pips. This then determines how many
        // ticks are drawn
        void setOuterTickValues(std::vector<float> values)
        {
            outerValues.clear();
            outerValues = values;
        }

        void setDrawPips(bool drawPips)
        {
            this->drawPips = drawPips;
        }

    private:

        OuterSlider outerSlider;
        InnerSlider innerSlider;

        DoubleStackLAF laf;

        void initialiseSliders(juce::Slider::SliderStyle sliderStyle)
        {
            outerSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
            innerSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);

            outerSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
            innerSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
        }

        void drawTick(float radius, float angle, int centreX, int centreY, juce::Graphics &g, juce::Colour colour)
        {
            juce::Path tick;

            float tickOuterOffset = radius * 0.01f;
            float tickWidth = radius * 0.02f;
            float tickLength = radius * 0.05f;

            g.setColour(colour);
            tick.addRectangle(0 - tickWidth / 2, -radius + tickOuterOffset, tickWidth, tickLength);
            g.fillPath(tick, juce::AffineTransform::rotation(
                    angle).translated(centreX, centreY));
        }
        void drawText(float radius, float angle, int centreX, int centreY, juce::Graphics &g, juce::Colour colour, const juce::String text)
        {
            // Note: the angle values are shifted by pi / 2 radians due to the way the slider calculates its angles.
            // We would expect to see a normal circle calculated from the horizontal to the right i.e 3 o'clock. This
            // means that we have to shift the angle calculation by 90deg, or pi/2 rads.
            auto x = cos(angle - pi / 2) * radius + centreX;
            auto y = sin(angle - pi / 2) * radius + centreY;

            g.setColour(colour);
            g.setFont(radius * 0.06);
            g.drawText(text, x - 25, y - 25, 50, 50, juce::Justification::centred);
        }

        float outerProportionalDiameter = 0.8f;
        float innerProportionalDiameter = 0.5f;

        bool mouseOverInnerSlider = false;
        bool drawPips = true;

        std::vector<float> innerValues, outerValues;
    };
}
