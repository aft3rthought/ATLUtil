

#pragma once

#include <cstddef>

namespace atl
{
    /*
     c_array_len: Returns the length of a C-style array
     */
    template <typename T, size_t N>
    constexpr size_t c_array_len(T(&)[N])
    {
        return N;
    }

    /*
     c_array_last: Returns by value the last element of a C-style array
     */
    template <typename T, size_t N>
    constexpr T c_array_last_by_value(T(&in_array)[N])
    {
        return in_array[N-1];
    }
    
    /*
     c_array_last_const_ref: Returns a reference to the last element of a C-style array
     */
    template <typename T, size_t N>
    constexpr T & c_array_last_by_ref(T(&in_array)[N])
    {
        return in_array[N-1];
    }
    
    /*
     wrapIdx: Returns the (i % N)'th item in an array of length N.
     */
    template <typename T, size_t N>
    T & wrap_idx(size_t index, T(&in_array)[N])
    {
        return in_array[index % in_array.size()];
    }

	/*
	c_array_byte_length: Returns the array's size in bytes.
	*/
	template <typename T, size_t N>
	size_t c_array_byte_length(T(&)[N])
	{
		return sizeof(T) * N;
	}
}
