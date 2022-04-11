/*
  ==============================================================================

    BDBottomPanel.cpp
    Created: 5 May 2021 3:30:01pm
    Author:  Olorin

  ==============================================================================
*/

#include "BottomPanel.h"

// Bottom Panel

BottomPanel::BottomPanel(BassDivisionAudioProcessor& inProcessor) : PanelBase(inProcessor),
processPanel(inProcessor), bandPanel(inProcessor)
{
    setSize(PROCESS_PANEL_WIDTH, PROCESS_PANEL_HEIGHT);

    addAndMakeVisible(processPanel);
    addAndMakeVisible(bandPanel);
    bandPanel.setVisible(false);

    processButton.setButtonStyle(DivisionVoid::Button::BarToggle);
    processButton.setButtonText("Process");
    processButton.setToggleState(true, juce::NotificationType::dontSendNotification);
    processButton.onClick = [this] ()
    {
        if (processButton.getToggleState() == true)
        {
            processButton.setToggleState(true, juce::NotificationType::dontSendNotification);
            bandButton.setToggleState(false, juce::NotificationType::dontSendNotification);
            processPanel.setVisible(true);
            bandPanel.setVisible(false);
        }

		if (processButton.getToggleState() == false)
		{
            processButton.setToggleState(true, juce::NotificationType::dontSendNotification);
		}
    };
    addAndMakeVisible(processButton);

    bandButton.setButtonStyle(DivisionVoid::Button::BarToggle);
    bandButton.setButtonText("Band");
    bandButton.onClick = [this] ()
    {
        if (bandButton.getToggleState() == true)
        {
            bandButton.setToggleState(true, juce::NotificationType::dontSendNotification);
            processButton.setToggleState(false, juce::NotificationType::dontSendNotification);
            processPanel.setVisible(false);
            bandPanel.setVisible(true);
        }

        if (bandButton.getToggleState() == false)
        {
            bandButton.setToggleState(true, juce::NotificationType::dontSendNotification);
        }
    };
    addAndMakeVisible(bandButton);
}

BottomPanel::~BottomPanel()
{
}

void BottomPanel::paint(juce::Graphics& g)
{
	
}

void BottomPanel::resized()
{
    auto xPivot = 10;
	
    processButton.setBounds(xPivot, getHeight() / 2 - 25, 120, 25);
    processButton.setTransform(juce::AffineTransform::rotation(M_PI_2 * 3, xPivot, getHeight() / 2 - 25));

    bandButton.setBounds(xPivot, getHeight() - 55, 120, 25);
    bandButton.setTransform(juce::AffineTransform::rotation(M_PI_2 * 3, xPivot, getHeight() - 55));

	processButton.setFont(fonts.getFont(DivisionVoidFonts::FontType::bold, 17));
	bandButton.setFont(fonts.getFont(DivisionVoidFonts::FontType::bold, 17));
}


// Process Panel

ProcessPanel::ProcessPanel(BassDivisionAudioProcessor& inProcessor) :
    PanelBase(inProcessor), eqPanel(inProcessor), compressorPanel(inProcessor)
{
    setSize(PROCESS_PANEL_WIDTH, PROCESS_PANEL_HEIGHT);

    addAndMakeVisible(eqPanel);
    addAndMakeVisible(compressorPanel);
}

void ProcessPanel::paint(juce::Graphics& g)
{

}

void ProcessPanel::resized()
{
    auto area = getLocalBounds();

    auto eqArea = area.removeFromLeft(PROCESS_PANEL_WIDTH / 2);
    eqPanel.setBounds(eqArea);
    compressorPanel.setBounds(area);
}

ProcessPanel::EqPanel::EqPanel(BassDivisionAudioProcessor &inProcessor) : PanelBase(inProcessor)
{
    // "Low Shelf", "200 Hz", "500 Hz", "1 KHz", "1.5 KHz",
    // "2.5 KHz", "High Shelf"
    for (int i = 0; i < 7; ++i)
    {
        sliders[i].setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
        sliders[i].setTextBoxStyle(juce::Slider::TextBoxBelow, false, 55, 12);
        sliders[i].setTextValueSuffix("dB");
        sliders[i].setLookAndFeel(&centreLAF);
        addAndMakeVisible(&sliders[i]);

        labels[i].setText(freqs[i], juce::dontSendNotification);
        labels[i].attachToComponent(&sliders[i], false);
        labels[i].setJustificationType(juce::Justification::centred);
        labels[i].setColour(0x1000281, juce::Colours::black);
        addAndMakeVisible(labels[i]);

        attachments[i] = MakeUnique<juce::AudioProcessorValueTreeState::SliderAttachment>(
                inProcessor.treeState, names[i], sliders[i]
                );

        for (auto i = 0; i < 2; ++i)
        {
            runes[i].setRune(i);
            addAndMakeVisible(runes[i]);
        }
    }
}

void ProcessPanel::EqPanel::paint(juce::Graphics &g)
{
    g.setFont(fonts.getFont(DivisionVoidFonts::FontType::bold, 25.0f));
    g.drawText("EQUALISER", 0, 0, getWidth() - 15, getHeight() - 15, juce::Justification::centredBottom);
}

void ProcessPanel::EqPanel::resized()
{
    const auto width = getWidth();
    const auto height = getHeight();

    const auto centreY = height / 2;

    const auto knobDiameter = height / 4;

    const auto border = 20;
    const auto yOffset = 30;

    sliders[0].setBounds((width / 6 * 1), border + yOffset, knobDiameter, knobDiameter);
    sliders[1].setBounds((width / 6 * 1.5), centreY + border, knobDiameter, knobDiameter);
    sliders[2].setBounds((width / 6 * 2), border + yOffset, knobDiameter, knobDiameter);
    sliders[3].setBounds((width / 6 * 2.5), centreY + border, knobDiameter, knobDiameter);
    sliders[4].setBounds((width / 6 * 3), border + yOffset, knobDiameter, knobDiameter);
    sliders[5].setBounds((width / 6 * 3.5), centreY + border, knobDiameter, knobDiameter);
    sliders[6].setBounds((width / 6 * 4), border + yOffset, knobDiameter, knobDiameter);


    runes[0].setBounds(40, centreY - 10, 20, 20);
    runes[1].setBounds(width - 60, centreY - 10, 20, 20);

    for (auto& label : labels)
    {
        label.setFont(fonts.getFont(DivisionVoidFonts::FontType::bold, 17));
    }
}

ProcessPanel::CompressorPanel::CompressorPanel(BassDivisionAudioProcessor &inProcessor) : PanelBase(inProcessor)
{
    // "ATTACK", "RELEASE", "RATIO", "THRESHOLD",
    // "KNEE", "HPF", "INPUT", "OUTPUT"
    for (int i = 0; i < 8; ++i)
    {
        sliders[i].setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
        sliders[i].setTextBoxStyle(juce::Slider::TextBoxBelow, false, 55, 12);

        labels[i].setText(names[i], juce::dontSendNotification);
        labels[i].attachToComponent(&sliders[i], false);
        labels[i].setJustificationType(juce::Justification::centred);
        labels[i].setColour(0x1000281, juce::Colours::black);

        attachments[i] = MakeUnique<juce::AudioProcessorValueTreeState::SliderAttachment>(
                inProcessor.treeState, "COMP_" + names[i], sliders[i]
                );
    }

    for (auto &slider : sliders)
    {
        addAndMakeVisible(slider);
    }

    for (auto &label : labels)
    {
        addAndMakeVisible(label);
    }

    sliders[0].setTextValueSuffix("ms");
    sliders[0].setLookAndFeel(&LAF);

    sliders[1].setTextValueSuffix("ms");
    sliders[1].setLookAndFeel(&LAF);

    sliders[2].setLookAndFeel(&LAF);

    sliders[3].setTextValueSuffix("dB");
    sliders[3].setLookAndFeel(&LAF);

    sliders[4].setLookAndFeel(&LAF);

    sliders[5].setTextValueSuffix("Hz");
    sliders[5].setLookAndFeel(&LAF);

    sliders[6].setTextValueSuffix("dB");
    sliders[6].setLookAndFeel(&centreLAF);

    sliders[7].setTextValueSuffix("dB");
    sliders[7].setLookAndFeel(&centreLAF);
}

void ProcessPanel::CompressorPanel::paint(juce::Graphics &g)
{
    g.setFont(fonts.getFont(DivisionVoidFonts::FontType::bold, 25.0f));
    g.drawText("COMPRESSOR", 0, 0, getWidth() - 15, getHeight() - 15, juce::Justification::centredBottom);
}

void ProcessPanel::CompressorPanel::resized()
{
    // Basic calculations for the layout. Realistically this should be dynamic, in a grid layout or using methods provided by juce, but this
    //should work for now
    const auto width = getWidth();
    const auto height = getHeight();

    const auto centreY = height / 2;

    const auto knobDiameter = height / 4;

    const auto border = 20;
    const auto yOffset = 30;

    // Threshold Ratio Attack Release
    // Input Knee HPF Output

    sliders[3].setBounds((width / 6 * 1), border + yOffset, knobDiameter, knobDiameter);
    sliders[2].setBounds((width / 6 * 2), border + yOffset, knobDiameter, knobDiameter);
    sliders[0].setBounds((width / 6 * 3), border + yOffset, knobDiameter, knobDiameter);
    sliders[1].setBounds((width / 6 * 4), border + yOffset, knobDiameter, knobDiameter);

    sliders[6].setBounds((width / 6 * 1), border + centreY, knobDiameter, knobDiameter);
    sliders[4].setBounds((width / 6 * 2), border + centreY, knobDiameter, knobDiameter);
    sliders[5].setBounds((width / 6 * 3), border + centreY, knobDiameter, knobDiameter);
    sliders[7].setBounds((width / 6 * 4), border + centreY, knobDiameter, knobDiameter);

    for (auto& label : labels)
    {
        label.setFont(fonts.getFont(DivisionVoidFonts::FontType::bold, 17));
    }
}


// Band Panel
BandPanel::BandPanel(BassDivisionAudioProcessor &inProcessor) : PanelBase(inProcessor), subPanel(inProcessor, "sub"),
                                                                lowPanel(inProcessor, "low"), midPanel(inProcessor, "mid"),
                                                                highPanel(inProcessor, "high"),
                                                                lowCrossoverPanel(inProcessor, "low"),
                                                                midCrossoverPanel(inProcessor, "mid"),
                                                                highCrossoverPanel(inProcessor, "high")
{
    setSize(PROCESS_PANEL_WIDTH, PROCESS_PANEL_HEIGHT);

    addAndMakeVisible(subPanel);
    addAndMakeVisible(lowPanel);
    addAndMakeVisible(midPanel);
    addAndMakeVisible(highPanel);
    addAndMakeVisible(lowCrossoverPanel);
    addAndMakeVisible(midCrossoverPanel);
    addAndMakeVisible(highCrossoverPanel);
}

void BandPanel::paint(juce::Graphics& g)
{

}

void BandPanel::resized()
{
    auto area = getBounds();

    area.removeFromRight(20);
    highPanel.setBounds(area.removeFromRight(DIST_PANEL_WIDTH)));
    highCrossoverPanel.setBounds(area.removeFromRight(XOVER_PANEL_WIDTH));
    midPanel.setBounds(area.removeFromRight(DIST_PANEL_WIDTH)));
    midCrossoverPanel.setBounds(area.removeFromRight(XOVER_PANEL_WIDTH));
    lowPanel.setBounds(area.removeFromRight(COMP_PANEL_WIDTH)));
    lowCrossoverPanel.setBounds(area.removeFromRight(XOVER_PANEL_WIDTH));
    subPanel.setBounds(area.removeFromRight(COMP_PANEL_WIDTH)));
}

BandPanel::BandCompPanel::BandCompPanel(BassDivisionAudioProcessor &inProcessor, juce::String bandName) :
        PanelBase(inProcessor), band(bandName.toUpperCase())
{
    // "ATTACK", "RELEASE", "RATIO", "THRESHOLD", "INPUT", "OUTPUT"
    for (int i = 0; i < 6; ++i)
    {
        sliders[i].setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
        sliders[i].setTextBoxStyle(juce::Slider::TextBoxBelow, false, 55, 12);

        labels[i].setText(names[i], juce::dontSendNotification);
        labels[i].attachToComponent(&sliders[i], false);
        labels[i].setJustificationType(juce::Justification::centred);
        labels[i].setColour(0x1000281, juce::Colours::black);

        attachments[i] = MakeUnique<juce::AudioProcessorValueTreeState::SliderAttachment>(
                inProcessor.treeState, band + "_COMP_" + names[i], sliders[i]
        );
    }

    for (auto &slider : sliders)
    {
        addAndMakeVisible(slider);
    }

    for (auto &label : labels)
    {
        addAndMakeVisible(label);
    }

    sliders[0].setTextValueSuffix("ms");
    sliders[0].setLookAndFeel(&LAF);

    sliders[1].setTextValueSuffix("ms");
    sliders[1].setLookAndFeel(&LAF);

    sliders[2].setLookAndFeel(&LAF);

    sliders[3].setTextValueSuffix("dB");
    sliders[3].setLookAndFeel(&LAF);

    sliders[4].setTextValueSuffix("dB");
    sliders[4].setLookAndFeel(&centreLAF);

    sliders[5].setTextValueSuffix("dB");
    sliders[5].setLookAndFeel(&centreLAF);

    soloButton.setToggleState(false, juce::dontSendNotification);
    soloButton.setButtonStyle(DivisionVoid::Button::ButtonStyle::BarToggle);
    soloButton.setButtonText("S");

    addAndMakeVisible(soloButton);

    soloButtonAttachment = MakeUnique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
            inProcessor.treeState, band + "_SOLO", soloButton
            );
}

void BandPanel::BandCompPanel::paint(juce::Graphics &g)
{
    g.setFont(fonts.getFont(DivisionVoidFonts::FontType::bold, 25.0f));
    g.drawText(band, 0, 0, getWidth(), getHeight() - 15, juce::Justification::centredBottom);
}

void BandPanel::BandCompPanel::resized()
{
    auto area = getBounds();

    const auto width = getWidth();
    const auto height = getHeight();

    const auto centreX = width / 2 + 20;
    const auto centreY = height / 2;

    const auto gainKnobSize = height / 5.5;

    const auto border = 0;

    auto row1 = 35;
    auto row2 = 125;
    auto row3 = 215;

    // "ATTACK", "RELEASE", "RATIO", "THRESHOLD", "INPUT", "OUTPUT"
    sliders[0].setBounds(centreX - gainKnobSize, row1, gainKnobSize, gainKnobSize);
    sliders[1].setBounds(centreX, row1, gainKnobSize, gainKnobSize);
    sliders[2].setBounds(centreX, row2, gainKnobSize, gainKnobSize);
    sliders[3].setBounds(centreX - gainKnobSize, row2, gainKnobSize, gainKnobSize);
    sliders[4].setBounds(centreX - gainKnobSize, row3, gainKnobSize, gainKnobSize);
    sliders[5].setBounds(centreX, row3, gainKnobSize, gainKnobSize);

    for (auto& label : labels) {
        label.setFont(fonts.getFont(DivisionVoidFonts::FontType::bold, 17));
    }

    const auto buttonWidth = 25;
    const auto buttonOffset = 10;

    soloButton.setBounds(buttonOffset + buttonWidth + 15, height - buttonOffset - buttonWidth, buttonWidth, buttonWidth);
    soloButton.setFont(fonts.getFont(DivisionVoidFonts::FontType::bold, 17));
}

BandPanel::BandDistortionPanel::BandDistortionPanel(BassDivisionAudioProcessor &inProcessor, juce::String bandName) :
        PanelBase(inProcessor), band(bandName.toUpperCase())
{
    // "GAIN", "MIX", "TYPE", "EDGE"
    for (int i = 0; i < 4; ++i)
    {
        sliders[i].setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
        sliders[i].setTextBoxStyle(juce::Slider::TextBoxBelow, false, 55, 12);

        labels[i].setText(names[i], juce::dontSendNotification);
        labels[i].attachToComponent(&sliders[i], false);
        labels[i].setJustificationType(juce::Justification::centred);
        labels[i].setColour(0x1000281, juce::Colours::black);

        attachments[i] = MakeUnique<juce::AudioProcessorValueTreeState::SliderAttachment>(
                inProcessor.treeState, band + "_DISTORTION_" + names[i], sliders[i]
        );
    }

    for (auto &slider : sliders)
    {
        addAndMakeVisible(slider);
    }

    for (auto &label : labels)
    {
        addAndMakeVisible(label);
    }

    sliders[0].setTextValueSuffix("dB");
    sliders[0].setLookAndFeel(&centreLAF);

    sliders[1].setTextValueSuffix("%");
    sliders[1].setLookAndFeel(&LAF);

    sliders[2].setLookAndFeel(&centreLAF);

    sliders[3].setLookAndFeel(&LAF);

    soloButton.setToggleState(false, juce::dontSendNotification);
    soloButton.setButtonStyle(DivisionVoid::Button::ButtonStyle::BarToggle);
    soloButton.setButtonText("S");

    addAndMakeVisible(soloButton);

    soloButtonAttachment = MakeUnique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
            inProcessor.treeState, band + "_SOLO", soloButton
    );

    /*for (auto i = 0; i < 2; ++i)
    {
        runes[i].setRune(i + 3);
        //addAndMakeVisible(runes[i]);
    }*/

}

void BandPanel::BandDistortionPanel::paint(juce::Graphics &g)
{
    g.setFont(fonts.getFont(DivisionVoidFonts::FontType::bold, 25.0f));
    g.drawText(band, 0, 0, getWidth(), getHeight() - 15, juce::Justification::centredBottom);
}

void BandPanel::BandDistortionPanel::resized()
{
    // "GAIN", "MIX", "TYPE", "EDGE"
    auto bounds = getLocalBounds();

    const auto width = getWidth();
    const auto height = getHeight();

    const auto centreY = height / 2;

    const auto paramKnobSize = height / 5;

    const auto borderX = 10;
    const auto borderY = 20;
    const auto yOffset = 30;

    sliders[0].setBounds(borderX, borderY + yOffset, paramKnobSize, paramKnobSize);
    sliders[3].setBounds(width - borderX - paramKnobSize, borderY + yOffset, paramKnobSize, paramKnobSize);
    sliders[1].setBounds(borderX, borderY + centreY, paramKnobSize, paramKnobSize);
    sliders[2].setBounds(width - borderX - paramKnobSize, borderY + centreY, paramKnobSize, paramKnobSize);

    for (auto& label : labels) {
        label.setFont(fonts.getFont(DivisionVoidFonts::FontType::bold, 17));
    }

    const auto buttonWidth = 25;
    const auto buttonOffset = 10;

    soloButton.setBounds(buttonOffset, height - buttonOffset - buttonWidth, buttonWidth, buttonWidth);
    soloButton.setFont(fonts.getFont(DivisionVoidFonts::FontType::bold, 17));
}

BandPanel::BandCrossoverPanel::BandCrossoverPanel(BassDivisionAudioProcessor &inProcessor, juce::String bandName) :
        PanelBase(inProcessor), band(bandName.toUpperCase())
{
    slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 55, 12);
    slider.setTextValueSuffix("Hz");
    slider.setLookAndFeel(&centreLAF);
    addAndMakeVisible(&slider);

    label.setText(band + " X-over", juce::dontSendNotification);
    label.attachToComponent(&slider, true);
    label.setJustificationType(juce::Justification::centred);
    label.setColour(0x1000281, juce::Colours::black);
    addAndMakeVisible(label);

    attachment = MakeUnique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            inProcessor.treeState, band + "_XOVER", slider
            );

    int index = 0;

    if (band == "MID") index = 2;
    if (band == "HIGH") index = 4;

    for (auto i = 0; i < 2; ++i)
    {
        runes[i].setRune(i + index);
        addAndMakeVisible(runes[i]);
    }
}

void BandPanel::BandCrossoverPanel::paint(juce::Graphics &g)
{
}

void BandPanel::BandCrossoverPanel::resized()
{
    auto height = getHeight();
    auto width = getWidth();

    auto knobSize = 40;

    slider.setBounds(width / 2 - knobSize / 2, height / 2 - knobSize / 2, knobSize, knobSize);

    label.setFont(fonts.getFont(DivisionVoidFonts::FontType::bold, 15));

    auto runeSize = 25;
    runes[0].setBounds(width / 2 - runeSize / 2, height / 4 * 1 - runeSize / 2, runeSize, runeSize);
    runes[1].setBounds(width / 2 - runeSize / 2, height / 4 * 3 - runeSize / 2, runeSize, runeSize);
}