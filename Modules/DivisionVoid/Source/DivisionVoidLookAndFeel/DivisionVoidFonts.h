//
// Created by Conor on 08/03/2022.
//

#pragma once

using namespace SharedResources;

class DivisionVoidFonts
{
public:
    DivisionVoidFonts()
    {
        norseBold = juce::Font(juce::Typeface::createSystemTypefaceFor(Norsebold_otf,
                                                                       Norsebold_otfSize));
    }

    enum class FontType
    {
        bold
    };

    juce::Font getFont(FontType type, float height)
    {
        juce::Font *fontToUse = nullptr;
        if (type == FontType::bold) fontToUse = &norseBold;
        else { jassertfalse; } // Font type has not been specified

        fontToUse->setHeight(height);

        return *fontToUse;
    }

private:

    juce::Font norseBold;
};
