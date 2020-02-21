

#pragma once

#include <type_traits>

namespace atl
{
    /* 
     * atl
     * utility functions for determining the number of bits required to represent an integral type
     */

	template <typename integer_type>
	constexpr integer_type compile_time_log2(integer_type x)
	{
		return x == 1 ? 0 : (1 + compile_time_log2(x >> 1));
	}

	static_assert(compile_time_log2<unsigned>(8) == 3, "compile_time_log2: 8 should be 3");
	static_assert(compile_time_log2<unsigned>(32) == 5, "compile_time_log2: 32 should be 5");
	static_assert(compile_time_log2<unsigned>(64) == 6, "compile_time_log2: 64 should be 6");
	static_assert(compile_time_log2<unsigned>(std::numeric_limits<unsigned>::max()) == (sizeof(unsigned) * 8 - 1), "compile_time_log2: unsigned max");

	template <typename integer_type>
	constexpr bool fits_in_n_bits(integer_type input_value, typename std::make_unsigned<integer_type>::type shift)
	{
		typename std::make_unsigned<integer_type>::type value = input_value;
		return (input_value >> shift) == 0;
	}

	static_assert(fits_in_n_bits<unsigned>(0, 0), "fits_in_n_bits: assert 0 fits in 0");
	static_assert(!fits_in_n_bits<unsigned>(1, 0), "fits_in_n_bits: assert 1 does not fit in 0");
	static_assert(fits_in_n_bits<unsigned>(1, 1), "fits_in_n_bits: assert 1 fits in 1");
	static_assert(!fits_in_n_bits<unsigned>(2, 1), "fits_in_n_bits: assert 2 does not fit in 1");
	static_assert(fits_in_n_bits<unsigned>(2, 2), "fits_in_n_bits: assert 2 fits in 2");
	static_assert(!fits_in_n_bits<unsigned>(15, 3), "fits_in_n_bits: assert 15 does not fit in 3");
	static_assert(fits_in_n_bits<unsigned>(15, 4), "fits_in_n_bits: assert 15 fits in 4");
	static_assert(!fits_in_n_bits<unsigned>(16, 4), "fits_in_n_bits: assert 16 does not fit in 4");
	static_assert(fits_in_n_bits<unsigned>(16, 5), "fits_in_n_bits: assert 16 fits in 5");
	static_assert(fits_in_n_bits<unsigned>(32767, 31), "fits_in_n_bits: assert 32767 fits in 31");
	static_assert(fits_in_n_bits<unsigned>(std::numeric_limits<unsigned>::max(), sizeof(unsigned) * 8), "fits_in_n_bits: unsigned max");

	template <typename integer_type>
	constexpr integer_type integer_midpoint(integer_type lower, integer_type upper)
	{
		return integer_type{(lower + upper) / 2};
	}

	template <typename integer_type>
	constexpr integer_type binary_search_step(std::integral_constant<std::size_t, 0>, integer_type lower, integer_type upper, integer_type value) {
		return fits_in_n_bits(value, lower) ? lower : upper;
	}

	static_assert(binary_search_step<unsigned>(std::integral_constant<std::size_t, 0>(), 3, 4, 4) == 3, "binary_search_step, final step: 3,4,4 should return 3");
	static_assert(binary_search_step<unsigned>(std::integral_constant<std::size_t, 0>(), 3, 4, 8) == 4, "binary_search_step, final step: 3,4,8 should return 4");
	static_assert(binary_search_step<unsigned>(std::integral_constant<std::size_t, 0>(), 4, 5, 16) == 5, "binary_search_step, final step: 4,5,16 should return 5");

	template <typename integer_type, std::size_t step_count>
	constexpr integer_type binary_search_step(std::integral_constant<size_t, step_count>, integer_type lower, integer_type upper, integer_type value) {
		return
			fits_in_n_bits(value, integer_midpoint(lower, upper)) ?
			 binary_search_step<integer_type>(std::integral_constant<size_t, step_count-1>(), lower, integer_midpoint(lower, upper), value) :
			 binary_search_step<integer_type>(std::integral_constant<size_t, step_count-1>(), integer_midpoint(lower, upper), upper, value);
	}

	template <typename integer_type>
	constexpr integer_type bits_required_for_integer(integer_type integer_value) {
		using unsigned_type = std::make_unsigned<integer_type>::type;
		constexpr unsigned_type unsigned_type_bits = sizeof(unsigned_type) * 8;
		using step_count_type = std::integral_constant<size_t, compile_time_log2(unsigned_type_bits)>;
		return static_cast<integer_type>(binary_search_step<unsigned_type>(step_count_type(), 0, unsigned_type_bits, integer_value));
	}
	
	static_assert(bits_required_for_integer<unsigned>(0) == 0, "bits_required_for_integer: 0 should return 0");
	static_assert(bits_required_for_integer<unsigned>(1) == 1, "bits_required_for_integer: 1 should return 1");
	static_assert(bits_required_for_integer<unsigned>(2) == 2, "bits_required_for_integer: 2 should return 2");
	static_assert(bits_required_for_integer<unsigned>(4) == 3, "bits_required_for_integer: 4 should return 3");
	static_assert(bits_required_for_integer<unsigned>(8) == 4, "bits_required_for_integer: 8 should return 4");
	static_assert(bits_required_for_integer<unsigned>(498734) == 19, "bits_required_for_integer: 498734 should return 19");
	static_assert(bits_required_for_integer<unsigned>(std::numeric_limits<unsigned>::max()) == sizeof(unsigned) * 8, "bits_required_for_integer: unsigned max");

    /* 
     * atl
     * bit string definition
     */

    using bit_string_byte_type = unsigned char;

    struct input_bit_string_type
    {
        input_bit_string_type()
		{
			ptr = begin = end = nullptr;
			bit_offset = 0;
		}
        
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
        output_bit_string_type()
		{
			ptr = begin = end = nullptr;
			bit_offset = 0;
		}
        
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
            auto bits_required = bits_required_for_integer(max - min);
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
        using definitely_unsigned_integer_type = typename std::make_unsigned<integer_type>::type;
		using definitely_signed_integer_type = typename std::make_signed<integer_type>::type;
		if(std::is_signed<integer_type>::value)
		{
			value = ((definitely_signed_integer_type)((definitely_unsigned_integer_type)result >> 1) ^ -((definitely_signed_integer_type)result & 0b1));
		}
		else
		{
			value = result;
		}
        return true;
    }
    
    /* 
     * atl
     * bit string writing functions
     */
    
    template <typename state_generator_callable_type>
    bool bit_string_write_bits(state_generator_callable_type & state_generator, const bit_string_byte_type * buffer, size_t num_bits)
    {
#pragma message("TODO: Investigate working in a memset")
#pragma message("TODO: Bits set in the incoming value outside of the range (IE writing 2 bits of 0xFF) will end up leaking into the whole range. Intended?")
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
    bool bit_string_write_bytes(state_generator_callable_type & state_generator, const bit_string_byte_type * buffer, uint32_t buffer_size_in_bytes)
    {
        return bit_string_write_bits(state_generator, buffer, buffer_size_in_bytes * 8);
    }
    
    
    
    template <typename value_type, typename state_generator_callable_type>
    bool bit_string_write_value(state_generator_callable_type & state_generator, const value_type & value)
    {
        if(std::is_same<value_type, bool>::value) {
#pragma message("TODO: This causes code generation of weird stuff")
            auto value_to_write = (bool)value ? (unsigned char)0b1 : (unsigned char)0b0;
            return bit_string_write_bits(state_generator, &value_to_write, 1);
        }
        else
        {
            return bit_string_write_bytes(state_generator, (const bit_string_byte_type *)&value, sizeof(value_type));
        }
    }
    
    template <typename value_type, typename state_generator_callable_type>
    bool bit_string_write_values(state_generator_callable_type & state_generator, const value_type & value)
    {
        return bit_string_write_value(state_generator, value);
    }
    
    template <typename value_type, typename state_generator_callable_type, typename ... arg_pack_type>
    bool bit_string_write_values(state_generator_callable_type & state_generator, const value_type & value, arg_pack_type&&... arg_pack)
    {
        if(bit_string_write_value(state_generator, value)) return bit_string_write_values(state_generator, arg_pack...);
        return false;
    }
    
    template <typename array_type, size_t array_size, typename state_generator_callable_type>
    bool bit_string_write_c_array(state_generator_callable_type & state_generator, const array_type (&in_array)[array_size])
    {
        return bit_string_write_bytes(state_generator, (const bit_string_byte_type *)in_array, array_size * sizeof(array_type));
    }
    
    template <typename integer_type, typename state_generator_callable_type>
    bool bit_string_write_ranged_integer(state_generator_callable_type & state_generator, integer_type value, integer_type min, integer_type max)
    {
        if(max == min)
        {
            return true;
        }
        else if(max > min)
        {
            auto range = max - min;
            value -= min;
            return bit_string_write_bits(state_generator, (const bit_string_byte_type *)&value, bits_required_for_integer(range));
        }
        return false;
    }
    
    template <typename integer_type, typename state_generator_callable_type>
    bool bit_string_write_chunked_integer(state_generator_callable_type & state_generator, integer_type value, size_t chunk_size)
    {
        using local_integer_type = typename std::make_unsigned<integer_type>::type;
        constexpr local_integer_type local_integer_size_bits = sizeof(local_integer_type) * 8;
        local_integer_type current_value = std::is_signed<integer_type>::value ? ((value << 1) ^ (value >> (local_integer_size_bits - 1))) : value;
        local_integer_type mask = std::numeric_limits<local_integer_type>::max() >> (local_integer_size_bits - chunk_size);
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
