
#pragma once

namespace atl
{
	template <typename region_value_type>
	struct region_type
	{
		region_value_type * begin_pointer;
		region_value_type * end_pointer;

		region_value_type * begin() const { return begin_pointer; }
		region_value_type * end() const { return end_pointer; }
		region_value_type * front() const { return begin_pointer; }
		region_value_type * back() const { return end_pointer - 1; }
		int size() const { return int{end_pointer - begin_pointer}; }
		bool empty() const { return end_pointer <= begin_pointer; }

		operator region_type<const region_value_type> () const
		{
			return region_type<const region_value_type>{begin_pointer, end_pointer};
		}
	};

	template <typename region_value_type>
	region_type<region_value_type> region(region_value_type * begin_pointer, region_value_type * end_pointer)
	{
		return region_type<region_value_type>{begin_pointer, end_pointer};
	}

	template <typename region_value_type, typename integer_type>
	region_type<region_value_type> region_n(region_value_type * begin_pointer, integer_type count)
	{
		return region_type<region_value_type>{begin_pointer, begin_pointer + count};
	}

	template <typename region_value_type, size_t c_array_extent>
	region_type<region_value_type> region_from_array(region_value_type (&c_array)[c_array_extent])
	{
		return region_type<region_value_type>{c_array, c_array + c_array_extent};
	}

	template <typename region_value_type, size_t c_array_extent>
	region_type<region_value_type> region_from_string_literal_minus_terminator(region_value_type(&c_array)[c_array_extent])
	{
		return region_from_array(c_array).remove_tail(1);
	}
}
