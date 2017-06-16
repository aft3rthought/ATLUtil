

#pragma once

namespace atl
{
    /* 
     * atl
     * utility functions for determining the number of bits required to represent an integral type
     */

    template <typename integer_type>
    constexpr integer_type compile_time_bits_required_for_integer_value(integer_type input_value)
    {
        typename std::make_unsigned<integer_type>::type value = input_value;
        return 1 + (value == 0 || value == 1 ? 0 : compile_time_bits_required_for_integer_value(value >> 1));
    }
    
    template <typename integer_type>
    size_t bits_required_for_integer_value(integer_type input_value)
    {
#pragma message("TODO: size-neutral binary search")
        typename std::make_unsigned<integer_type>::type value = input_value;
        size_t bits_required = 0;
        while(value > 0)
        {
            value = value >> 1;
            bits_required++;
        }
        return bits_required;
    }

    /* 
     * atl
     * bit string definition
     */

    using bit_string_byte_type = unsigned char;

    struct input_bit_string_type
    {
        input_bit_string_type() {};
        
        input_bit_string_type(const bit_string_byte_type * attach_begin, const bit_string_byte_type * attach_end)
        {
            ptr = begin = attach_begin;
            end = attach_end;
            bit_offset = 0;
        }
        
        const bit_string_byte_type * begin;
        const bit_string_byte_type * end;
        const bit_string_byte_type * ptr;
        size_t bit_offset;
        
        bool has_bytes() const { return ptr < end; }
        size_t bytes_remaining() const { return has_bytes() ? (size_t)(end - ptr) : 0; }
    };
    
    struct output_bit_string_type
    {
        output_bit_string_type() {};
        
        output_bit_string_type(bit_string_byte_type * attach_begin, bit_string_byte_type * attach_end)
        {
            ptr = begin = attach_begin;
            end = attach_end;
            bit_offset = 0;
        }
        
        bit_string_byte_type * begin;
        bit_string_byte_type * end;
        bit_string_byte_type * ptr;
        size_t bit_offset;
        
        bool has_bytes() const { return ptr < end; }
        size_t bytes_remaining() const { return has_bytes() ? (size_t)(end - ptr) : 0; }
    };
    
    static inline bool bit_string_skip_to_next_byte(input_bit_string_type & bit_string_state)
    {
        if(!bit_string_state.has_bytes()) return false;
        if(bit_string_state.bit_offset != 0)
        {
            bit_string_state.ptr++;
            bit_string_state.bit_offset = 0;
        }
        return true;
    }
    
    static inline bool bit_string_skip_bits(input_bit_string_type & bit_string_state, size_t number_of_bits)
    {
        auto new_offset = bit_string_state.bit_offset + number_of_bits;
        auto num_bytes = new_offset / 8;
        if(bit_string_state.bytes_remaining() < num_bytes) return false;
        bit_string_state.ptr += num_bytes;
        bit_string_state.bit_offset = new_offset % 8;
        return true;
    }
    
    static inline bool bit_string_skip_bytes(input_bit_string_type & bit_string_state, size_t number_of_bytes)
    {
        return bit_string_skip_bits(bit_string_state, number_of_bytes * 8);
    }
    
    /* 
     * atl
     * bit string reading functions
     */

    static inline bool bit_string_read_bits(input_bit_string_type & bit_string_state, unsigned char * output_buffer, size_t num_bits_to_read)
    {
#pragma message("TODO: Investigate working in a memset")
        auto max_bits_remaining = bit_string_state.bytes_remaining() * 8 + bit_string_state.bit_offset;
        if(num_bits_to_read > max_bits_remaining) return false;
        
        auto bits_read = (size_t)0;
        while(bits_read < num_bits_to_read)
        {
            auto offset_in_read = (bits_read % 8);
            
            if(offset_in_read == 0)
                *output_buffer = 0;
            
            auto bits_remaining = num_bits_to_read - bits_read;
            auto bits_to_next_data_byte = std::min(8 - offset_in_read, bits_remaining);
            auto bits_available_in_byte = 8 - bit_string_state.bit_offset;
            auto bits_to_read = std::min(bits_to_next_data_byte, bits_available_in_byte);
            
            *output_buffer |= ((*bit_string_state.ptr >> bit_string_state.bit_offset) & ((bit_string_byte_type)0xFF >> (8 - bits_to_read))) << offset_in_read;
            
            bit_string_state.bit_offset += bits_to_read;
            bits_read += bits_to_read;
            if(bits_read % 8 == 0)
                output_buffer++;
            
            if(bit_string_state.bit_offset == 8)
            {
                bit_string_state.ptr++;
                bit_string_state.bit_offset = 0;
            }
        }
        return true;
    }
    
    static inline bool bit_string_read_bytes(input_bit_string_type & bit_string_state, bit_string_byte_type * output_buffer, size_t output_buffer_size_in_bytes)
    {
        return bit_string_read_bits(bit_string_state, output_buffer, output_buffer_size_in_bytes * 8);
    }
    
    template <typename value_type>
    bool bit_string_read_value(input_bit_string_type & bit_string_state, value_type & value)
    {
        return bit_string_read_bytes(bit_string_state, (bit_string_byte_type *)&value, sizeof(value_type));
    }

    template <typename value_type = bool>
    bool bit_string_read_value(input_bit_string_type & bit_string_state, bool & in_value)
    {
        unsigned char result = 0x00;
        if(bit_string_read_bits(bit_string_state, &result, 1))
        {
            in_value = result != 0x00;
            return true;
        }
        return false;
    }

    template <typename value_type>
    bool bit_string_read_values(input_bit_string_type & bit_string_state, value_type & value)
    {
        return bit_string_read_value(bit_string_state, value);
    }
    
    template <typename value_type, typename ... arg_pack_type>
    bool bit_string_read_values(input_bit_string_type & bit_string_state, value_type & value, arg_pack_type&&... arg_pack)
    {
        if(bit_string_read_value(bit_string_state, value)) return bit_string_read_values(bit_string_state, arg_pack...);
        return false;
    }
    
    template <typename array_type, size_t array_size>
    bool bit_string_read_c_array(input_bit_string_type & bit_string_state, array_type (&output_array)[array_size])
    {
        return bit_string_read_bytes(bit_string_state, (bit_string_byte_type *)output_array, array_size * sizeof(array_type));
    }
    
    template <typename integer_type>
    bool bit_string_read_ranged_integer(input_bit_string_type & bit_string_state, integer_type & value, integer_type min, integer_type max)
    {
        if(max == min)
        {
            value = min;
            return true;
        }
        else if(max > min)
        {
            auto bits_required = bits_required_for_integer_value(max - min);
            auto result = (integer_type)0;
            if(bit_string_read_bits(bit_string_state, (bit_string_byte_type *)&result, bits_required))
            {
                value = min + result;
                return true;
            }
        }
        return false;
    }
    
    template <typename integer_type>
    bool bit_string_read_chunked_integer(input_bit_string_type & bit_string_state, integer_type & value, size_t chunk_size)
    {
        integer_type result = 0;
        integer_type chunk;
        size_t current_bit = 0;
        bool chunk_flag;
        if(!bit_string_read_value(bit_string_state, chunk_flag)) return false;
        while(chunk_flag)
        {
            chunk = 0;
            if(!bit_string_read_bits(bit_string_state, (bit_string_byte_type *)&chunk, chunk_size)) return false;
            result += chunk << current_bit;
            current_bit += chunk_size;
            if(!bit_string_read_value(bit_string_state, chunk_flag)) return false;
        }
        value = std::is_signed<integer_type>::value ? ((result << 1) ^ (result >> 31)) : result;
        return true;
    }
    
    /* 
     * atl
     * bit string writing functions
     */
    
    template <typename state_generator_callable_type>
    bool bit_string_write_bits(state_generator_callable_type state_generator, const bit_string_byte_type * buffer, size_t num_bits)
    {
#pragma message("TODO: Investigate working in a memset")
        output_bit_string_type & bit_string_state = state_generator(num_bits);
        auto bits_written = (size_t)0;
        while(bits_written < num_bits)
        {
            if(bit_string_state.bit_offset == 8)
            {
                bit_string_state.ptr++;
                bit_string_state.bit_offset = 0;
                if(!bit_string_state.has_bytes()) return false;
            }
            
            auto offset_in_write = (bits_written % 8);
            auto bits_remaining = num_bits - bits_written;
            auto bits_to_next_data_byte = std::min(8 - offset_in_write, bits_remaining);
            auto bits_available_in_byte = 8 - bit_string_state.bit_offset;
            auto bits_to_write = std::min(bits_to_next_data_byte, bits_available_in_byte);
            
            *bit_string_state.ptr |= (((*buffer) >> offset_in_write) << bit_string_state.bit_offset);
            
            bit_string_state.bit_offset += bits_to_write;
            bits_written += bits_to_write;
            if(bits_written % 8 == 0)
                buffer++;
        }
        return true;
    }
    
    template <typename state_generator_callable_type>
    bool bit_string_write_bytes(state_generator_callable_type state_generator, const bit_string_byte_type * buffer, uint32_t buffer_size_in_bytes)
    {
        return bit_string_write_bits(state_generator, buffer, buffer_size_in_bytes * 8);
    }
    
    template <typename value_type, typename state_generator_callable_type>
    bool bit_string_write_value(state_generator_callable_type state_generator, const value_type & value)
    {
        return bit_string_write_bytes(state_generator, (const bit_string_byte_type *)&value, sizeof(value_type));
    }
    
    template <typename value_type = bool, typename state_generator_callable_type>
    bool bit_string_write_value(state_generator_callable_type state_generator, const bool & value)
    {
        unsigned char value_to_write = value ? 0xFF : 0x00;
        return bit_string_write_bits(state_generator, &value_to_write, 1);
    }
    
    template <typename value_type, typename state_generator_callable_type>
    bool bit_string_write_values(state_generator_callable_type state_generator, const value_type & value)
    {
        return bit_string_write_value(state_generator, value);
    }
    
    template <typename value_type, typename state_generator_callable_type, typename ... arg_pack_type>
    bool bit_string_write_values(state_generator_callable_type state_generator, const value_type & value, arg_pack_type&&... arg_pack)
    {
        if(bit_string_write_value(state_generator, value)) return bit_string_write_values(state_generator, arg_pack...);
        return false;
    }
    
    template <typename array_type, size_t array_size, typename state_generator_callable_type>
    bool bit_string_write_c_array(state_generator_callable_type state_generator, const array_type (&in_array)[array_size])
    {
        return bit_string_write_bytes(state_generator, (const bit_string_byte_type *)in_array, array_size * sizeof(array_type));
    }
    
    template <typename integer_type, typename state_generator_callable_type>
    bool bit_string_write_ranged_integer(state_generator_callable_type state_generator, integer_type value, integer_type min, integer_type max)
    {
        if(max == min)
        {
            return true;
        }
        else if(max > min)
        {
            auto range = max - min;
            value -= min;
            return bit_string_write_bits(state_generator, value, bits_required_for_non_negative_integer_value(range));
        }
        return false;
    }
    
    template <typename integer_type, typename state_generator_callable_type>
    bool bit_string_write_chunked_integer(state_generator_callable_type state_generator, integer_type value, size_t chunk_size)
    {
        using local_integer_type = typename std::make_unsigned<integer_type>::type;
        local_integer_type current_value = std::is_signed<integer_type>::value ? ((value << 1) ^ (value >> 31)) : value;
        local_integer_type mask = std::numeric_limits<local_integer_type>::max() >> (sizeof(local_integer_type) * 8 - chunk_size);
        while(current_value > 0)
        {
            local_integer_type write_val = current_value & mask;
            if(!bit_string_write_value(state_generator, true)) return false;
            if(!bit_string_write_bits(state_generator, (const bit_string_byte_type *)&write_val, chunk_size)) return false;
            current_value = current_value >> chunk_size;
        }
        if(!bit_string_write_value(state_generator, false)) return false;
        return true;
    }
}
