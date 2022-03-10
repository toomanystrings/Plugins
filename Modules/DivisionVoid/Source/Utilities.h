//
// Created by Conor Foran on 10/03/2022.
//

#pragma once

#include <iostream>
#include <iomanip>
#include <memory>

constexpr double pi    = 3.141592653589793238462643383279;
constexpr double twoPi = 2.0 * pi;

// Little helpers to reduce the amount of typing for smart pointers
template <typename t>
using UniquePtr = std::unique_ptr<t>;

template<typename T, typename... Args>
UniquePtr<T> MakeUnique(Args&&... args)
{
    return std::make_unique<T>(std::forward<Args>(args)...);
}

inline double getScaledValueFromNormalizedValue(double input, double lowerBound, double upperBound, double skew = 1.0)
{
    jassert(input >= 0.0 && input <= 1.0);

    double skewedInput = std::pow(input, skew);

    return (skewedInput * (lowerBound - upperBound)) + upperBound;
}

inline double getNormalizedValueFromScaledRange(double input, double lowerBound, double upperBound, double skew = 1.0)
{
    double output = (input - lowerBound) / (upperBound - lowerBound);

    return pow(output, 1.0 / skew);
}
