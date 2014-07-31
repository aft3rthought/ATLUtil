

#pragma once

#include <vector>
#include <algorithm>

namespace atl
{
    /*
     * value_in_vector: Return true if value is in vector. Passes value by value, and compares using ==, so ideal for numeric types.
     */
    template <typename T>
    bool value_in_vector(T value, const std::vector<T> & vector)
    {
        return std::find(vector.begin(), vector.end(), value) != vector.end();
    }
}