

#pragma once

namespace atl
{
	// Default conversions
	//
	// These are placeholders. Currently they just static cast.
	// What contract do these enforce?
	//

	template <typename naked_type>
	constexpr float default_int_to_float(const naked_type & value)
	{
		return static_cast<float>(int{value});
	}

	template <typename naked_type>
	constexpr float default_unsigned_to_float(const naked_type & value)
	{
		return static_cast<float>(unsigned{value});
	}

	template <typename naked_type>
	constexpr float default_u32_to_float(const naked_type & value)
	{
		return static_cast<float>(uint32_t{value});
	}
	
	template <typename naked_type>
	constexpr int default_double_to_int(const naked_type & value)
	{
		return static_cast<int>(double{value});
	}

	template <typename naked_type>
	constexpr unsigned default_int_to_unsigned(const naked_type & value)
	{
		return static_cast<unsigned>(int{value});
	}

	template <typename naked_type>
	constexpr int default_float_to_int(const naked_type & value)
	{
		return static_cast<int>(float{value});
	}

	template <typename naked_type>
	constexpr long default_float_to_long(const naked_type & value)
	{
		return static_cast<long>(float{value});
	}	

	template <typename naked_type>
	constexpr unsigned default_float_to_unsigned(const naked_type & value)
	{
		return static_cast<unsigned>(float{value});
	}	

	template <typename naked_type>
	constexpr int default_unsigned_to_int(const naked_type & value)
	{
		return static_cast<int>(unsigned{value});
	}

	// Safe comparators
	//
	// These sacrifice extra instructions in order to supply correctness.
	// For instance, comparing a signed and unsigned integer is made safe by doing an extra check to see if the signed integer is negative.
	//

	template <typename wrapped_type>
	struct safe_comparator_type
	{
		wrapped_type value;
		explicit safe_comparator_type(wrapped_type passthrough_value) : value(passthrough_value) {}
	};

	template <typename wrapped_type>
	safe_comparator_type<wrapped_type> safe_comparator(const wrapped_type & passthrough_value) { return safe_comparator_type<wrapped_type>(passthrough_value); }

	inline bool operator < (const safe_comparator_type<unsigned> & lhs, const safe_comparator_type<int> & rhs)
	{
		if(rhs.value < 0)  return false;
		return lhs.value < static_cast<unsigned>(rhs.value);
	}

	inline bool operator < (const safe_comparator_type<int> & lhs, const safe_comparator_type<unsigned> & rhs)
	{
		if(lhs.value < 0) return true;
		return static_cast<unsigned>(lhs.value) < rhs.value;
	}

}
