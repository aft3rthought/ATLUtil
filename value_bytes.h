

#pragma once

#include <algorithm>

namespace atl
{
    template <typename value_type>
    constexpr char * value_bytes_begin(value_type & value)
    {
        return reinterpret_cast<char *>(&value);
    }
    
    template <typename value_type, size_t value_type_size = sizeof(value_type)>
    constexpr char * value_bytes_end(value_type & value)
    {
        return reinterpret_cast<char *>(&value) + value_type_size;
    }
    
    template <typename value_type, typename generator_type, typename generator_result_type>
    void fill_value_bytes_with_generator(value_type & value, generator_type & generator)
    {
        auto bytes_start = value_bytes_begin(value);
        auto bytes_end = value_bytes_end(value);
        const auto generator_result_size = sizeof(generator_result_type);
        generator_result_type generated_bytes = generator();
        int generated_bytes_available = generator_result_size;
        while(bytes_start != bytes_end)
        {
            if(generated_bytes_available == 0)
            {
                generated_bytes = generator();
                generated_bytes_available += generator_result_size;
            }
            *bytes_start = generated_bytes;
            ++bytes_start;
            generated_bytes = generated_bytes >> 8;
            generated_bytes_available--;
        }
    }
    
    template <typename value_type, typename random_engine_type>
    void fill_value_bytes_with_std_random(value_type & value, random_engine_type & engine)
    {
        fill_value_bytes_with_generator<value_type, random_engine_type, typename random_engine_type::result_type>(value, engine);
    }

    template <typename value_type>
    void fill_value_bytes(value_type & value, char fill_value)
    {
        std::fill(value_bytes_begin(value), value_bytes_end(value), fill_value);
    }
    
    template <typename value_type>
    void zero_value_bytes(value_type & value)
    {
        fill_value_bytes(value, 0x00);
    }
    
    template <typename value_type>
    bool compare_value_bytes(value_type & valueA, value_type & valueB)
    {
        return std::equal(value_bytes_begin(valueA), value_bytes_end(valueA), value_bytes_begin(valueB));
    }
}
