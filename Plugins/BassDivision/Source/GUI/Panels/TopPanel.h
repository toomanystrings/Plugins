//
// Created by Conor Foran on 12/04/2022.
//

#pragma once

#include "../PanelBase.h"

struct IrPanel : public PanelBase, private juce::Timer
{
    IrPanel(BassDivisionAudioProcessor& inProcessor);
    ~IrPanel();

    void paint (juce::Graphics&) override;
    void resized() override;

    void timerCallback() override;

    void updateImpulseResponseComboBox();

private:

    DivisionVoid::Button engageImpulseResponse;
    DivisionVoid::Button loadImpulseResponse;

    juce::ComboBox impulseResponseComboBox;

    juce::Slider highPassIRKnob;
    juce::Label highPassLabel;

    UniquePtr<juce::AudioProcessorValueTreeState::ButtonAttachment> engageIRAttachment;

    juce::String currentImpulseResponseName{ "No IR Loaded" };
    juce::String presetDisplayName{ "" };

    DivisionVoid::ComboBoxLookAndFeel lookAndFeel;

    DivisionVoidFonts fonts;

    BassDivisionAudioProcessor& audioProcessor;
};

struct PresetPanel : public PanelBase, private juce::Timer
{
    PresetPanel (BassDivisionAudioProcessor&);
    ~PresetPanel();

    void paint (juce::Graphics& g) override;

    void resized() override;

    void timerCallback() override;

    void updatePresetComboBox();

private:

    DivisionVoid::Button savePreset;
    juce::TextButton loadPreset;

    juce::ComboBox presetComboBox;

    juce::String currentPresetName {"Factory Settings"};
    juce::String presetDisplayName { "Factory Settings" };

    DivisionVoid::ComboBoxLookAndFeel lookAndFeel;
    DivisionVoidFonts fonts;

    BassDivisionAudioProcessor& audioProcessor;
};

class TopPanel : public PanelBase
{
public:
    TopPanel(BassDivisionAudioProcessor& inProcessor);
    ~TopPanel() = default;

    void paint(juce::Graphics& g) override;
    void resized() override;

private:

    PresetPanel presetPanel;
    IrPanel irPanel;
};