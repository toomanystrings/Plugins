//
// Created by Conor Foran on 10/10/2024.
//

#pragma once

namespace DivisionVoid
{
    class FeedBackCombFilter
    {
    public:
        FeedBackCombFilter() = default;
        ~FeedBackCombFilter() = default;

        float processSample(float x)
        {

        }

    private:

        DivisionVoid::FractionalDelay<float> fractionalDelay;
    };
}
