//
// Created by Conor on 08/03/2022.
//

#pragma once

class DivisionVoidFonts
{
public:
    DivisionVoidFonts()
    {
        norseBold = juce::Font(juce::Typeface::createSystemTypefaceFor(SharedResources::Norsebold_oft,
                                                                       SharedResources::Norsebold_oftSize));
    }

private:

    juce::Font norseBold;
};
