
#pragma once

#include <cstdint>
#include <cstddef>

namespace atl
{
	// Default conversions
	//
	// These are placeholders. Currently they just static cast.
	// What contract do these enforce?
	//
	// Currently thinking: Allow compile time decision about doing bounds checking

	template <typename naked_type>
	constexpr float default_int_to_float(const naked_type & value)
	{
		return static_cast<float>(int{value});
	}

	template <typename naked_type>
	constexpr double default_int_to_double(const naked_type& value)
	{
		return static_cast<double>(int{value});
	}

	template <typename naked_type>
	constexpr float default_unsigned_to_float(const naked_type & value)
	{
		return static_cast<float>(unsigned{value});
	}

	template <typename naked_type>
	constexpr double default_unsigned_to_double(const naked_type& value)
	{
		return static_cast<double>(unsigned{value});
	}

	template <typename naked_type>
	constexpr float default_u32_to_float(const naked_type & value)
	{
		return static_cast<float>(uint32_t{value});
	}

	template <typename naked_type>
	constexpr float default_u64_to_float(const naked_type & value)
	{
		return static_cast<float>(uint64_t{value});
	}

	template <typename naked_type>
	constexpr int default_u64_to_int(const naked_type & value)
	{
		return static_cast<int>(uint64_t{value});
	}

	template <typename naked_type>
	constexpr unsigned default_u64_to_unsigned(const naked_type& value)
	{
		return static_cast<unsigned>(uint64_t{value});
	}

	template <typename naked_type>
	constexpr int default_double_to_int(const naked_type & value)
	{
		return static_cast<int>(double{value});
	}

	template <typename naked_type>
	constexpr unsigned default_double_to_unsigned(const naked_type& value)
	{
		return static_cast<unsigned>(double{value});
	}

	template <typename naked_type>
	constexpr unsigned default_int_to_unsigned(const naked_type & value)
	{
		return static_cast<unsigned>(int{value});
	}

	template <typename naked_type>
	constexpr uint64_t default_int_to_u64(const naked_type& value)
	{
		return static_cast<uint64_t>(int{value});
	}

	template <typename naked_type>
	constexpr size_t default_int_to_size_t(const naked_type& value)
	{
		return static_cast<size_t>(int{ value });
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
	constexpr uint64_t default_float_to_u64(const naked_type& value)
	{
		return static_cast<uint64_t>(float{value});
	}

	template <typename naked_type>
	constexpr unsigned default_size_t_to_unsigned(const naked_type& value)
	{
		return static_cast<unsigned>(size_t{value});
	}

	template <typename naked_type>
	constexpr float default_size_t_to_float(const naked_type& value)
	{
		return static_cast<float>(size_t{value});
	}

	template <typename naked_type>
	constexpr double default_size_t_to_double(const naked_type& value)
	{
		return static_cast<double>(size_t{value});
	}	

	template <typename naked_type>
	constexpr int default_ptrdiff_to_int(const naked_type& value)
	{
		return static_cast<int>(std::ptrdiff_t{value});
	}	

	template <typename naked_type>
	constexpr uint64_t default_size_t_to_u64(const naked_type& value)
	{
		return static_cast<uint64_t>(size_t{value});
	}

	template <typename naked_type>
	constexpr int default_size_t_to_int(const naked_type& value)
	{
		return static_cast<int>(size_t{value});
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

	template <typename naked_type>
	constexpr char default_unsigned_to_char(const naked_type& value)
	{
		return static_cast<char>(unsigned{value});
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
		constexpr explicit safe_comparator_type(wrapped_type passthrough_value) : value(passthrough_value) {}
	};

	template <typename wrapped_type>
	constexpr safe_comparator_type<wrapped_type> safe_comparator(const wrapped_type & passthrough_value) { return safe_comparator_type<wrapped_type>(passthrough_value); }

	constexpr bool operator < (const safe_comparator_type<uint32_t> & lhs, const safe_comparator_type<int32_t> & rhs)
	{
		if(rhs.value < 0)  return false;
		return lhs.value < static_cast<unsigned>(rhs.value);
	}

	constexpr bool operator < (const safe_comparator_type<int32_t> & lhs, const safe_comparator_type<uint32_t> & rhs)
	{
		if(lhs.value < 0) return true;
		return static_cast<unsigned>(lhs.value) < rhs.value;
	}

	constexpr bool operator < (const safe_comparator_type<uint32_t>& lhs, const safe_comparator_type<int64_t>& rhs)
	{
		if(rhs.value < 0)  return false;
		return lhs.value < static_cast<unsigned>(rhs.value);
	}

	constexpr bool operator < (const safe_comparator_type<int64_t>& lhs, const safe_comparator_type<uint32_t>& rhs)
	{
		if(lhs.value < 0) return true;
		return static_cast<unsigned>(lhs.value) < rhs.value;
	}

	constexpr bool operator == (const safe_comparator_type<uint32_t> & lhs, const safe_comparator_type<int32_t> & rhs)
	{
		if(rhs.value < 0)  return false;
		return lhs.value == static_cast<unsigned>(rhs.value);
	}

	constexpr bool operator == (const safe_comparator_type<int32_t> & lhs, const safe_comparator_type<uint32_t> & rhs)
	{ 
		if(lhs.value < 0) return false;
		return static_cast<unsigned>(lhs.value) == rhs.value;
	}

	constexpr bool operator == (const safe_comparator_type<uint32_t>& lhs, const safe_comparator_type<int64_t>& rhs)
	{
		if(rhs.value < 0)  return false;
		return lhs.value == static_cast<unsigned>(rhs.value);
	}

	constexpr bool operator == (const safe_comparator_type<int64_t>& lhs, const safe_comparator_type<uint32_t>& rhs)
	{
		if(lhs.value < 0) return false;
		return static_cast<unsigned>(lhs.value) == rhs.value;
	}

	static_assert(safe_comparator(int32_t{-1}) < safe_comparator(uint32_t{1}) == true, "int vs unsigned < (1)");
	static_assert(safe_comparator(int32_t{1}) < safe_comparator(uint32_t{1}) == false, "int vs unsigned < (2)");
	static_assert(safe_comparator(int32_t{1}) < safe_comparator(uint32_t{2}) == true, "int vs unsigned < (3)");

	static_assert(safe_comparator(int32_t{-1}) == safe_comparator(uint32_t{1}) == false, "int vs unsigned == (1)");
	static_assert(safe_comparator(int32_t{1}) == safe_comparator(uint32_t{2}) == false, "int vs unsigned == (2)");
	static_assert(safe_comparator(int32_t{1}) == safe_comparator(uint32_t{1}) == true, "int vs unsigned == (3)");

	static_assert(safe_comparator(uint32_t{1}) < safe_comparator(int32_t{-1}) == false, "unsigned vs int < (1)");
	static_assert(safe_comparator(uint32_t{1}) < safe_comparator(int32_t{1}) == false, "unsigned vs int < (2)");
	static_assert(safe_comparator(uint32_t{1}) < safe_comparator(int32_t{2}) == true, "unsigned vs int < (3)");

	static_assert(safe_comparator(uint32_t{1}) == safe_comparator(int32_t{-1}) == false, "unsigned vs int == (1)");
	static_assert(safe_comparator(uint32_t{2}) == safe_comparator(int32_t{1}) == false, "unsigned vs int == (2)");
	static_assert(safe_comparator(uint32_t{1}) == safe_comparator(int32_t{1}) == true, "unsigned vs int == (3)");
}
