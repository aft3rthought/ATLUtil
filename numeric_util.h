

#pragma once

#include <limits>

namespace atl
{
    template <typename result_type, typename input_type, typename on_failure_callable_type>
    result_type integer_cast(input_type input_value, on_failure_callable_type on_failure_callable)
    {
        const auto result = (result_type)input_value;
        if((input_type)result != input_value) on_failure_callable();
        return result;
    }
}
