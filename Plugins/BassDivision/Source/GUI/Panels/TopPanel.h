//
// Created by Conor Foran on 12/04/2022.
//

#pragma once

#include "../PanelBase.h"

struct PresetPanel : public PanelBase
{

};

struct IrPanel : public PanelBase
{

};

class TopPanel : public PanelBase
{
public:
    TopPanel(BassDivisionAudioProcessor& inProcessor);
    ~TopPanel() = default;

    void paint(juce::Graphics& g) override;
    void resized() override;

private:


};