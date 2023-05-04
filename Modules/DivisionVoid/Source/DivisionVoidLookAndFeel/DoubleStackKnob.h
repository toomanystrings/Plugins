//
// Created by Conor Foran on 27/02/2023.
//

#pragma once

namespace DivisionVoid
{
    class DoubleStackKnob : public juce::Component
    {
    public:
        DoubleStackKnob()
        {
            setSize(getParentWidth(), getParentHeight());

            outerSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
            innerSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);

            outerSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
            innerSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

            outerSlider.setLookAndFeel(&laf);
            innerSlider.setLookAndFeel(&laf);


            addAndMakeVisible(outerSlider);
            addAndMakeVisible(innerSlider);
        }
        ~DoubleStackKnob() = default;

        void paint (juce::Graphics&) override
        {

        }

        void resized() override
        {
            outerSlider.setBounds(getBounds());
            innerSlider.setBounds(getBounds().reduced(100));
        }

    private:

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

        private:
            juce::Point<int> centre;
            int radius;
        };

        InnerSlider innerSlider;
        juce::Slider outerSlider;

        DivisionVoid::DoubleStackLAF laf;
    };


}
