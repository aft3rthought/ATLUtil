

#pragma once

namespace atl
{
	/*
	 integer_divide_rounding_up
	 Divide value by divisor, rounding up
	*/
	template <class T>
	constexpr T integer_divide_rounding_up(T value, T divisor)
	{
		return (value + divisor - 1) / divisor;
	}

	template <class number_type>
	struct numeric_range_type
	{
		const number_type v0, v1, span;

		constexpr number_type unmap(number_type value) const { return (value - v0) / span; }
		constexpr number_type map(number_type value) const { return v0 + value * span; }
		constexpr number_type clamp(number_type value) const { return value < v0 ? v0 : (value > v1 ? v1 : value); }
	};

	template <class number_type>
	constexpr numeric_range_type<number_type> numeric_range(number_type v0, number_type v1)
	{
		return numeric_range_type<number_type>{v0, v1, v1 - v0};
	}

	template <class number_type>
	number_type remap(number_type value, const numeric_range_type<number_type> & input_range, const numeric_range_type<number_type> & output_range)
	{
		return output_range.map(input_range.unmap(value));
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
