#pragma once

namespace DivisionVoid
{
    class Segment : public juce::Component
    {
    public:
        Segment(const juce::Colour& c) : colour(c) {}

        void paint(juce::Graphics& g) override
        {
            g.setColour(juce::Colours::black);

            if (isOn)
                g.setColour(colour);

            const auto delta = 2.0f;
            const auto bounds = getLocalBounds().toFloat().reduced(delta);
            const auto segmentFillBounds = juce::Rectangle<float>{
                    bounds.getX(),
                     bounds.getY(),
                     bounds.getWidth(),
                     bounds.getHeight()};

            g.fillRoundedRectangle(segmentFillBounds, 1);
            g.setColour(juce::Colours::black);
            g.drawRoundedRectangle(segmentFillBounds, 1, 1);

            if (isOn)
            {
                g.setGradientFill(juce::ColourGradient{
                    colour.withAlpha(0.3f),
                     segmentFillBounds.getCentre(),
                     colour.withLightness(1.5f).withAlpha(0.0f),
                     {},
                     true});
                g.fillRoundedRectangle(segmentFillBounds.expanded(delta), 1);
            }
        }

        void setState(const bool state) { isOn = state; }

    private:
        bool isOn = false;
        juce::Colour colour{};
    };

class SegmentedDisplayMeter : public juce::Component, juce::Timer
    {
    public:
        enum class Orientation {Vertical, Horizontal};

        SegmentedDisplayMeter(/*std::function<float()>&& valueFunction*/) /*: valueSupplier(std::move(valueFunction))*/
        {
            orientation = Orientation::Vertical;
        }

        void paint(juce::Graphics& g) override
        {
//            const auto value = juce::jmap(
//                    valueSupplier(),
//                    -60.0f,
//                    6.0f,
//                    0.0f,
//                    1.0f);

            // Purely for testing at the minute
            auto value = 1.0f;

            for (int i = 0; i < totalNumberOfSegments; ++i)
            {
                segments[i]->setState(false);

                if (value >= static_cast<float>(i + 1) / totalNumberOfSegments)
                    segments[i]->setState(true);
            }
        }

        void resized() override
        {
            // Set colour gradient
            const auto bounds = getLocalBounds().toFloat();
            juce::ColourGradient gradient {
                juce::Colours::darkgrey,
                bounds.getBottomLeft(),
                juce::Colours::red,
                bounds.getTopLeft(),
                false};
            gradient.addColour(0.3f, juce::Colours::grey);
            gradient.addColour(0.8f, juce::Colours::darkred);

            // Find the correct ratio
            const auto ratio = 2.5f / 30.0f;

            totalNumberOfSegments = juce::jmax(bounds.getWidth(), bounds.getHeight()) * ratio;

            // Here, we must determine the direction to draw the meter
            const auto segmentHeight = getLocalBounds().getHeight() / totalNumberOfSegments;
            const auto segmentWidth = getLocalBounds().getWidth() / totalNumberOfSegments;

            auto segmentBounds = getLocalBounds();
            segments.clear();
            for (auto i = 0; i < totalNumberOfSegments; i++)
            {
                auto segment = MakeUnique<Segment>(gradient.getColourAtPosition(static_cast<double>(i) / totalNumberOfSegments));
                addAndMakeVisible(segment.get());
                if (orientation == Orientation::Vertical)
                    segment->setBounds(segmentBounds.removeFromBottom(segmentHeight));
                else
                    segment->setBounds(segmentBounds.removeFromLeft(segmentWidth));

                segments.push_back(std::move(segment));
            }
        }

        void timerCallback() override { repaint(); }

        // Needs to be set in the resized() method.
        void setOrientation(Orientation orientation) { this->orientation = orientation; }

    private:
        //std::function<float()> valueSupplier;
        std::vector<UniquePtr<Segment>> segments;
        int totalNumberOfSegments = 25;

        Orientation orientation;
    };
}