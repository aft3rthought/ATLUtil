

#pragma once

namespace atl
{
    /*
     int_div_up
     Divide an integer by an integer, rounding up
     */
    template <class T>
    constexpr T int_div_up(T numerator, T denominator)
    {
        return (numerator - 1) / numerator + 1;
    }
    
    /*
     clamp
     Clamp value to the range [targetMin, targetMax]
     */
    template <class T>
    constexpr T clamp(T value, T targetMin, T targetMax)
    {
        return value < targetMin ? targetMin : (value > targetMax ? targetMax : value);
    }
    
    /*
     mapf
     Generate an interpolation factor from [0, 1] using value, originMin, and originMax.
     Then interpolate from targetMin to targetMax using that value.
     Standard precision floating point version.
     */
    constexpr float mapf(float v, float originMin, float originMax, float targetMin, float targetMax)
    {
        return v <= originMin ? targetMin :
               v >= originMax ? targetMax : targetMin + (targetMax - targetMin) * ((v - originMin) / (originMax - originMin));
    }

    /*
     mapd
     Generate an interpolation factor from [0, 1] using value, originMin, and originMax.
     Then interpolate from targetMin to targetMax using that value.
     Double-precision floating point version.
     */
    constexpr double mapd(double v, double originMin, double originMax, double targetMin, double targetMax)
    {
        return v <= originMin ? targetMin :
               v >= originMax ? targetMax : targetMin + (targetMax - targetMin) * ((v - originMin) / (originMax - originMin));
    }

    /*
     mapf_unclamped
     Generate an interpolation factor using value, originMin, and originMax.
     Then interpolate from targetMin to targetMax using that value.
     If value lies outside of originMin, originMax, the interpolation factor will not
     lie within the range [0, 1], giving results outside of the range [targetMin, targetMax].
     Standard precision floating point version.
     */
    constexpr float mapf_unclamped(float v, float originMin, float originMax, float targetMin, float targetMax)
    {
        return targetMin + (targetMax - targetMin) * ((v - originMin) / (originMax - originMin));
    }

    /*
     mapd_unclamped
     Generate an interpolation factor using value, originMin, and originMax.
     Then interpolate from targetMin to targetMax using that value.
     If value lies outside of originMin, originMax, the interpolation factor will not
     lie within the range [0, 1], giving results outside of the range [targetMin, targetMax].
     Double-precision floating point version.
     */
    constexpr double mapd_unclamped(double v, double originMin, double originMax, double targetMin, double targetMax)
    {
        return targetMin + (targetMax - targetMin) * ((v - originMin) / (originMax - originMin));
    }
    
    /*
     step_map
     A simple step function.
     Return ifBelow if value is less than about, otherwise return ifEqualToOrAbove.
     */
    template <class T>
    constexpr T map_step(T value, T about, T ifBelow, T ifEqualToOrAbove)
    {
        return value < about ? ifBelow : ifEqualToOrAbove;
    }
    
    /*
     interpolateF
     Interpolate between from and to using interpolation factor percent.
     Standard precision floating point version.
     */
    constexpr float interpf(float from, float to, float percent)
    {
        return from + percent * (to - from);
    }

    /*
     interpolateD
     Interpolate between from and to using interpolation factor percent.
     Double-precision floating point version.
     */
    constexpr double interpd(double from, double to, double percent)
    {
        return from + percent * (to - from);
    }
}
