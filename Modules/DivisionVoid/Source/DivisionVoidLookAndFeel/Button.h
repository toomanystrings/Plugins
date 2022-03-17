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

