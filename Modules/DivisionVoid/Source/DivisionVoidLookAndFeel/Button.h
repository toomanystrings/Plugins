//
// Created by Conor Foran on 17/03/2022.
//

#pragma once

namespace DivisionVoid
{
class Button : public juce::Button, private juce::Timer
{
public:
    Button() = default;
    ~Button() = default;

    enum ButtonStyle
    {
        Bar,
        BarToggle,
        SlidingToggle
    };

    void setButtonText(const String& newText)
    {
        juce::Button::setButtonText(newText);
    }

    void setButtonStyle(ButtonStyle style)
    {
        buttonStyle = style;
        resized();
        repaint();
    }

    ButtonStyle getButtonStyle()
    {
        return buttonStyle;
    }

    void setFont(juce::Font font)
    {
        buttonFont = font;
        resized();
        repaint();
    }

    juce::Font getFont()
    {
        return buttonFont;
    }

    void setCornerCutoff(float cutoff)
    {
        jassert(cutoff >= 0.1f && cutoff < 0.5f);
        cornerCutoff = cutoff;
    }

private:

    void mouseMove(const juce::MouseEvent& e) override
    {
        // Might use this to change cursor style when interacting with the sliding toggle
        juce::Button::mouseMove(e);
    }

    void mouseDown(const juce::MouseEvent& e) override
    {
        if(!isEnabled()) { return; }
        colourInterpolator.setCurrentAndTargetValue(1.0f);

        if (buttonStyle == ButtonStyle::Bar)
        {
            startTimerHz(ANIMATION_FPS);
            setToggleState(true, juce::sendNotification);
        }
        else if (buttonStyle == ButtonStyle::BarToggle)
        {
            startTimerHz(ANIMATION_FPS);
            juce::Button::mouseDown(e);
        }
        else if (buttonStyle == ButtonStyle::SlidingToggle)
        {
            // This will have some weird behavior depending on where in the button was clicked.
            // This is going to be implemented later, as I currently have no use for a sliding toggle,
            // but it will find a place in a later project.
        }

    }

    void mouseDrag(const juce::MouseEvent& e) override
    {
        if (!isEnabled()) { return; }

        if (buttonStyle != ButtonStyle::SlidingToggle)
        {
            juce::Button::mouseDrag(e);
        }
        else
        {
            // Will come into play as and when we implement the sliding toggle
        }
    }

    void mouseUp  (const juce::MouseEvent& e) override
    {
        if(!isEnabled()) { return; }

        if(buttonStyle == ButtonStyle::Bar)
        {
            setToggleState(false, juce::dontSendNotification);
        }
        else if(buttonStyle == ButtonStyle::BarToggle)
        {
            juce::Button::mouseUp(e);
        }
        else if (buttonStyle == ButtonStyle::SlidingToggle)
        {
            // This will have some weird behavior depending on where in the button was clicked.
            // This is going to be implemented later, as I currently have no use for a sliding toggle,
            // but it will find a place in a later project.
        }
    }

    void enablementChanged() override
    {
        // Might be worth using to change colours if I wanted to use IDs, or could just remove this probably
    }

    void timerCallback() override
    {

    }

    void paintButton(juce::Graphics& g, bool isMouseOverButton, bool isButtonDown) override
    {
        int width  = getWidth();
        int height = getHeight();

        if (buttonStyle != ButtonStyle::SlidingToggle)
        {
            int cutoff = static_cast<int>(std::min(width, height) * scaleFactor);

            juce::Path p;
            //p.addRoundedRectangle(0, 0, width, height, CORNER_RADIUS, CORNER_RADIUS, false, !(isConnectedOnTop() || isConnectedOnRight()), !(isConnectedOnBottom() || isConnectedOnLeft()), false);
            p.startNewSubPath(0, 0);
            p.lineTo(width - cutoff, 0);
            p.lineTo(width, cutoff);
            p.lineTo(width, height);
            p.lineTo(cutoff, height);
            p.lineTo(0, height - cutoff);
            p.closeSubPath();

            juce::Colour background;
            juce::Colour foreground;
            juce::Colour text;

            if (buttonStyle != ButtonStyle::Bar)
            {
                if (getToggleState())
                {
                    background = findColour(DivisionVoid::highlightColourId);

                    if (hasKeyboardFocus(true) && wasFocusedByTab)
                    {
                        foreground = findColour(DivisionVoid::foregroundColourId);
                        text = juce::Colours::antiquewhite;
                    }
                    else
                    {
                        foreground = findColour(DivisionVoid::backgroundColourId);
                        text = juce::Colours::antiquewhite;
                    }
                }
                else
                {
                    background = findColour(DivisionVoid::foregroundColourId);

                    if (hasKeyboardFocus(true) && wasFocusedByTab)
                    {
                        foreground = findColour(DivisionVoid::highlightColourId);
                        text = juce::Colours::antiquewhite;
                    }
                    else
                    {
                        foreground = findColour(DivisionVoid::backgroundColourId);
                        text = juce::Colours::antiquewhite;
                    }
                }
            }
            else
            {
                background = findColour(DivisionVoid::foregroundColourId);

                if (hasKeyboardFocus(true) && wasFocusedByTab)
                {
                    foreground = findColour(DivisionVoid::highlightColourId);
                    text = juce::Colours::antiquewhite;
                }
                else
                {
                    foreground = findColour(DivisionVoid::backgroundColourId);
                    text = juce::Colours::antiquewhite;
                }
            }

            g.setColour(background.interpolatedWith(findColour(DivisionVoid::midgroundColourId), colourInterpolation.getNextValue()));
            g.fillPath(p);

            g.setColour(findColour(DivisionVoid::Colours::Black.getARGB()));
            float strokeWidth = 2.f;
            g.strokePath(p, juce::PathStrokeType(strokeWidth, juce::PathStrokeType::JointStyle::mitered));

            g.setColour(text);
            g.setFont(buttonFont);
            g.drawFittedText(getButtonText(), CORNER_RADIUS / 2, CORNER_RADIUS / 2, width - CORNER_RADIUS, height - CORNER_RADIUS, juce::Justification::centred, 1, 1.0f);
        }
        else
        {
            juce::Path p;
            p.addRoundedRectangle(0, 0, width, height, CORNER_CONFIG);
            g.setColour(findColour(DivisionVoid::midgroundColourId));
            g.fillPath(p);

            p.clear();

            p.addRoundedRectangle(trackArea.getX(), trackArea.getY(), trackArea.getWidth(), trackArea.getHeight(), 8, 8, false, true, true, false);
            g.setColour(findColour(DivisionVoid::backgroundColourId));
            g.fillPath(p);

            p.clear();

            p.addRoundedRectangle(indicatorArea.getX(), indicatorArea.getY(), indicatorArea.getWidth(), indicatorArea.getHeight(), 8, 8, false, true, true, false);
            g.setColour(findColour(DivisionVoid::highlightColourId));
            g.fillPath(p);

            p.clear();

            p.addRoundedRectangle(thumbArea.getX(), thumbArea.getY(), thumbArea.getWidth(), thumbArea.getHeight(), 8, 8, false, true, true, false);
            g.setColour(findColour(DivisionVoid::foregroundColourId));
            g.fillPath(p);

            g.setColour(findColour(DivisionVoid::midgroundColourId));
            int strokeWidth = std::min(thumbArea.getWidth(), thumbArea.getHeight()) / 4;
            g.strokePath(p, (juce::PathStrokeType) strokeWidth);
            //juce::PathStrokeType::PathStrokeType(strokeWidth));
        }
    }

    void resized() override
    {

    }

    juce::LinearSmoothedValue<float> colourInterpolator;

    ButtonStyle buttonStyle;
    float cornerCutoff = 0.33f;

    juce::Font buttonFont;


};
}

