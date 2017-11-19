#pragma once

#include "ATLUtil/region.h"

namespace atl
{
	template <typename region_value_type>
	struct region_stack_type
	{
		region<region_value_type> internal_storage;
		region_value_type * internal_stack_head;

		region_stack_type() {}

		region_stack_type(const region<region_value_type> & in_storage)
		:
		internal_storage(in_storage),
		internal_stack_head(in_storage.begin())
		{}

		region_stack_type(const region<region_value_type> & in_storage, region_value_type * in_stack_head)
		:
		internal_storage(in_storage),
		internal_stack_head(in_stack_head)
		{}

		region_value_type & push() { return *internal_stack_head++; }
		void pop() { internal_stack_head--; }
		region_value_type * begin() const { return internal_storage.begin(); }
		region_value_type * end() const { return internal_stack_head; }
		region_value_type & bottom() const { return *internal_storage.begin(); }
		region_value_type & top() const { return *(internal_stack_head - 1); }
		size_t size() const { return internal_stack_head - internal_storage.begin(); }
		bool empty() const { return internal_stack_head <= internal_storage.begin(); }
		size_t capacity() const { return internal_storage.size(); }
		bool full() { return internal_stack_head >= internal_storage.end(); }
	};
}
