

#pragma once

#include <stddef.h>

namespace atl
{
    /*
     arrayLength: Returns the length of a C-style array
     */
    template <typename T, size_t N>
    constexpr size_t c_array_len(T(&)[N])
    {
        return N;
    }
    
    /*
     wrapIdx: Returns the (i % N)'th item in an array of length N.
     */
    template <typename T, size_t N>
    T & wrap_idx(size_t index, T(&in_array)[N])
    {
        return in_array[index % in_array.size()];
    }
    
    template <typename T>
    bool value_in_vector(T value, const std::vector<T> & vector)
    {
        return std::find(vector.begin(), vector.end(), value) != vector.end();
    }
}