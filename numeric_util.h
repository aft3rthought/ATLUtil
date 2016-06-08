

#pragma once

#include "ATLUtil/debug_break.h"
#include <limits>

#define atl_numeric_cast_assert atl_assert_fatal

namespace atl
{
    template <typename result_type, typename input_type>
    result_type ignore_sign(input_type input_value)
    {
        static_assert(sizeof(result_type) == sizeof(input_type), "Types must have the same size");
        return (result_type)input_value;
    }
    
    template <typename result_type, typename input_type>
    result_type integer(input_type);
    
    template <>
    inline unsigned int integer(int input_value)
    {
        atl_numeric_cast_assert(input_value >= 0, "Signed to unsigned conversion: Input value is negative");
        return (unsigned int)input_value;
    }

    template <>
    inline int integer(unsigned int input_value)
    {
        atl_numeric_cast_assert(input_value <= std::numeric_limits<int>::max(), "Unsigned to signed conversion: Input value is too large");
        return (int)input_value;
    }

    template <>
    inline int integer(unsigned long input_value)
    {
        atl_numeric_cast_assert(input_value <= std::numeric_limits<int>::max(), "Unsigned to signed conversion: Input value is too large");
        return (int)input_value;
    }
    
    template <>
    inline unsigned long integer(int input_value)
    {
        atl_numeric_cast_assert(input_value >= 0, "Signed to unsigned conversion: Input value is negative");
        return (unsigned long)input_value;
    }
}
