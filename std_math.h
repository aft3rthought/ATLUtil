

#pragma once

#include <limits>
#include <cmath>

namespace atl
{
    const static float sin_highest_value_phase = M_PI / 2.f;
    const static float sin_lowest_value_phase = M_PI * 3.f / 4.f;
    
    /*
     num_max
     Convenience function to get std::numeric_limits(decltype(num))::max
     */
    template <class T>
    constexpr T num_max(const T & num)
    {
        return std::numeric_limits<T>::max();
    }
    
    /*
     num_is_max
     Convenience function to test if num is equal to std::numeric_limits(decltype(num))::max
    */
    template <class T>
    constexpr bool num_is_max(const T & num)
    {
        return num == num_max(num);
    }
    
    /*
     set_num_max
     Convenience function to set num to the value std::numeric_limits(decltype(num))::max
     */
    template <class T>
    void set_num_max(T & num)
    {
        num = num_max(num);
    }
}
