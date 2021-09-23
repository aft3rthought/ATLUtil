

#pragma once

namespace atl
{
	// integer_divide_rounding_up
	// Divide value by divisor, rounding up
	template <class number_type>
	constexpr number_type integer_divide_rounding_up(number_type value, number_type divisor)
	{
		return (value + divisor - 1) / divisor;
	}

	// unsigned_subtract_with_floor
	// Computes a - b, with a minimum value of floor
	// For protecting against underflows, or for simply enforcing a floor.
	template <class number_type>
	constexpr number_type unsigned_subtract_with_floor(number_type a, number_type b, number_type floor)
	{
		return (b + floor < a) ? (a - b) : floor;
	}

	template <class number_type>
	struct numeric_range_type
	{
		const number_type v0, v1, span;

		constexpr number_type unmap(number_type value) const { return (value - v0) / span; }
		constexpr number_type map(number_type value) const { return v0 + value * span; }
		constexpr number_type clamp(number_type value) const { return value < v0 ? v0 : (value > v1 ? v1 : value); }
	};

	template <class scalar_type, class output_type>
	struct extrapolation_type
	{
		const output_type v0, span;

		constexpr output_type map(scalar_type value) const { return v0 + value * span; }
	};

	template <bool clamp_input, class input_type, class output_type>
	struct linear_transformation_type
	{
		const numeric_range_type<input_type> in;
		const extrapolation_type<input_type, output_type> out;

		constexpr output_type operator()(input_type value) const
		{
			return out.map(in.unmap(clamp_input ? in.clamp(value) : value));
		}
	};

	template <class number_type>
	constexpr numeric_range_type<number_type> numeric_range(number_type v0, number_type v1)
	{
		return numeric_range_type<number_type>{v0, v1, v1 - v0};
	}

	template <class scalar_type, class output_type>
	constexpr extrapolation_type<scalar_type, output_type> extrapolation(output_type v0, output_type v1)
	{
		return extrapolation_type<scalar_type, output_type>{v0, v1 - v0};
	}

	template <bool clamp_input, class input_type, class output_type>
	constexpr linear_transformation_type<clamp_input, input_type, output_type> linear_transformation(input_type i0, input_type i1, output_type o0, output_type o1)
	{
		return linear_transformation_type<clamp_input, input_type, output_type>{ numeric_range(i0, i1), extrapolation<input_type>(o0, o1) };
	}

	template <class number_type>
	number_type remap(number_type value, const numeric_range_type<number_type> & input_range, const numeric_range_type<number_type> & output_range)
	{
		return output_range.map(input_range.unmap(value));
	}

	template <class number_type>
	struct limited_math_operation_result_type
	{
		number_type value;
		bool reached_limit;
	};

	template <class number_type>
	limited_math_operation_result_type<number_type> limited_increment(number_type & value, number_type delta, number_type max)
	{
		value += delta;
		if(value >= max)
		{
			value = max;
			return {value, true};
		}
		return {value, false};
	}

	template <class number_type>
	limited_math_operation_result_type<number_type> limited_decrement(number_type & value, number_type delta, number_type min)
	{
		value -= delta;
		if(value <= min)
		{
			value = min;
			return {value, true};
		}
		return {value, false};
	}

	template <class number_type>
	limited_math_operation_result_type<number_type> move_to_limit(number_type & value, number_type delta, number_type limit)
	{
		if(value > limit)
			return limited_decrement(value, delta, limit);
		if(value < limit)
			return limited_increment(value, delta, limit);
		return {value, true};
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
