#pragma once

namespace atl
{
	template <typename region_value_type>
	struct region
	{
		region_value_type * begin_pointer;
		region_value_type * end_pointer;

		region() {}

		region(region_value_type * in_begin_pointer, region_value_type * in_end_pointer)
		:
		begin_pointer(in_begin_pointer),
		end_pointer(in_end_pointer)
		{}

		region_value_type * begin() const { return begin_pointer; }
		region_value_type * end() const { return end_pointer; }
		region_value_type * first() const { return begin_pointer; }
		region_value_type * last() const { return end_pointer - 1; }
		size_t size() const { return end_pointer - begin_pointer; }
		bool empty() const { return end_pointer <= begin_pointer; }
	};
}
