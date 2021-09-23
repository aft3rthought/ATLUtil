

#pragma once

#include <type_traits>
#include <limits>
#include <algorithm>

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
		return (shift >= sizeof(integer_type) * 8) || (input_value >> shift) == 0;
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
	static_assert(fits_in_n_bits<unsigned>(std::numeric_limits<unsigned>::max(), 32), "fits_in_n_bits: unsigned max");

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

    // Take a <=8 bit number out of a single byte, leaving it at the highest/leftmost side.
    constexpr unsigned char extract_sub_byte_from_byte(unsigned char input_byte, unsigned trim_left, unsigned trim_right)
    {
        const unsigned a = input_byte << trim_left;
        const unsigned b = a & (unsigned)0xFF;
        const unsigned c = b >> (trim_left + trim_right);
        return c;
    }
    static_assert(extract_sub_byte_from_byte(0b11111111, 1, 1) == (unsigned char)0b0111111, "Test extract_sub_byte_from_byte");
    static_assert(extract_sub_byte_from_byte(0b11111111, 0, 1) == (unsigned char)0b01111111, "Test extract_sub_byte_from_byte");
    static_assert(extract_sub_byte_from_byte(0b11111111, 1, 0) == (unsigned char)0b01111111, "Test extract_sub_byte_from_byte");
    static_assert(extract_sub_byte_from_byte(0b11111111, 3, 2) == (unsigned char)0b00000111, "Test extract_sub_byte_from_byte");
    static_assert(extract_sub_byte_from_byte(0b11111111, 2, 3) == (unsigned char)0b00000111, "Test extract_sub_byte_from_byte");
    static_assert(extract_sub_byte_from_byte(0b00000000, 0, 0) == (unsigned char)0b00000000, "Test extract_sub_byte_from_byte");
    static_assert(extract_sub_byte_from_byte(0b00000000, 1, 0) == (unsigned char)0b00000000, "Test extract_sub_byte_from_byte");
    static_assert(extract_sub_byte_from_byte(0b00000000, 0, 1) == (unsigned char)0b00000000, "Test extract_sub_byte_from_byte");
    static_assert(extract_sub_byte_from_byte(0b00000000, 1, 1) == (unsigned char)0b00000000, "Test extract_sub_byte_from_byte");
    static_assert(extract_sub_byte_from_byte(0b00000000, 3, 2) == (unsigned char)0b00000000, "Test extract_sub_byte_from_byte");
    static_assert(extract_sub_byte_from_byte(0b10000001, 0, 0) == (unsigned char)0b10000001, "Test extract_sub_byte_from_byte");
    static_assert(extract_sub_byte_from_byte(0b10000001, 1, 1) == (unsigned char)0b00000000, "Test extract_sub_byte_from_byte");
    static_assert(extract_sub_byte_from_byte(0b10000001, 3, 2) == (unsigned char)0b00000000, "Test extract_sub_byte_from_byte");
    static_assert(extract_sub_byte_from_byte(0b10100101, 1, 1) == (unsigned char)0b00010010, "Test extract_sub_byte_from_byte");
    static_assert(extract_sub_byte_from_byte(0b10100101, 2, 1) == (unsigned char)0b00010010, "Test extract_sub_byte_from_byte");
    static_assert(extract_sub_byte_from_byte(0b10100101, 1, 2) == (unsigned char)0b00001001, "Test extract_sub_byte_from_byte");

    constexpr unsigned two_way_unsigned_min(unsigned a, unsigned b) { return a < b ? a : b; }
    constexpr unsigned three_way_unsigned_min(unsigned a, unsigned b, unsigned c) { return two_way_unsigned_min(two_way_unsigned_min(a, b), c); }
    static_assert(three_way_unsigned_min(1, 2, 3) == 1, "Test three_way_unsigned_min");
    static_assert(three_way_unsigned_min(1, 3, 2) == 1, "Test three_way_unsigned_min");
    static_assert(three_way_unsigned_min(2, 1, 3) == 1, "Test three_way_unsigned_min");
    static_assert(three_way_unsigned_min(2, 3, 1) == 1, "Test three_way_unsigned_min");
    static_assert(three_way_unsigned_min(3, 2, 1) == 1, "Test three_way_unsigned_min");
    static_assert(three_way_unsigned_min(3, 1, 2) == 1, "Test three_way_unsigned_min");

    constexpr unsigned bitwise_copy_apply_inner_impl(const unsigned char input_byte,
                                                     unsigned char& output_byte,
                                                     const unsigned input_bits_remaining,
                                                     const unsigned input_bit_position_in_byte,
                                                     const unsigned output_bit_position_in_byte)
    {
        const unsigned remaining_bits_in_input_byte = 8 - input_bit_position_in_byte;
        const unsigned remaining_bits_in_output_byte = 8 - output_bit_position_in_byte;
        const unsigned bits_to_copy = three_way_unsigned_min(input_bits_remaining, remaining_bits_in_input_byte, remaining_bits_in_output_byte);
        const unsigned trim_right = input_bit_position_in_byte;
        const unsigned trim_left = 8 - trim_right - bits_to_copy;
        const unsigned char extracted_sub_byte = extract_sub_byte_from_byte(input_byte, trim_left, trim_right);
        const unsigned char shifted_sub_byte = extracted_sub_byte << output_bit_position_in_byte;
        output_byte |= shifted_sub_byte;
        return bits_to_copy;
    }
    constexpr unsigned bitwise_copy_apply(const unsigned char input_byte,
                                          unsigned char& output_byte,
                                          const unsigned input_bits_remaining,
                                          const unsigned input_bit_position_in_byte,
                                          const unsigned output_bit_position_in_byte)
    {
        return (input_bit_position_in_byte > 7 || output_bit_position_in_byte > 7) ? 0 : bitwise_copy_apply_inner_impl(input_byte, output_byte, input_bits_remaining, input_bit_position_in_byte, output_bit_position_in_byte);
    }

    struct bitwise_copy_apply_test_state
    {
        unsigned char output_byte;
        unsigned bits_copied;
    };
    constexpr bool operator == (const bitwise_copy_apply_test_state& a, const bitwise_copy_apply_test_state& b) { return a.output_byte == b.output_byte && a.bits_copied == b.bits_copied; }

    constexpr bitwise_copy_apply_test_state bitwise_copy_apply_test(const unsigned char input_byte, const unsigned char initial_output_byte, unsigned num_bits, unsigned input_pos, unsigned output_pos)
    {
        unsigned char output_byte = initial_output_byte;
        auto bits_copied = bitwise_copy_apply(input_byte, output_byte, num_bits, input_pos, output_pos);
        return {output_byte, bits_copied};
    }
    static_assert(bitwise_copy_apply_test(0b11111111, 0b00000000, 3, 8, 0).bits_copied == 0, "Test bitwise_copy_apply: Invalid bit position");
    static_assert(bitwise_copy_apply_test(0b11111111, 0b00000000, 3, 0, 8).bits_copied == 0, "Test bitwise_copy_apply: Invalid bit position");
    static_assert(bitwise_copy_apply_test(0b11111111, 0b00000000, 3, 8, 8).bits_copied == 0, "Test bitwise_copy_apply: Invalid bit position");
    static_assert(bitwise_copy_apply_test(0b00000001, 0b00000000, 1, 0, 0) == bitwise_copy_apply_test_state{0b00000001, 1}, "Test bitwise_copy_apply");
    static_assert(bitwise_copy_apply_test(0b00000001, 0b00000000, 3, 0, 0) == bitwise_copy_apply_test_state{0b00000001, 3}, "Test bitwise_copy_apply");
    static_assert(bitwise_copy_apply_test(0b11111111, 0b00000000, 3, 0, 0) == bitwise_copy_apply_test_state{0b00000111, 3}, "Test bitwise_copy_apply");
    static_assert(bitwise_copy_apply_test(0b11111111, 0b00000000, 3, 2, 0) == bitwise_copy_apply_test_state{0b00000111, 3}, "Test bitwise_copy_apply");
    static_assert(bitwise_copy_apply_test(0b11111111, 0b00000000, 3, 0, 2) == bitwise_copy_apply_test_state{0b00011100, 3}, "Test bitwise_copy_apply");
    static_assert(bitwise_copy_apply_test(0b11111111, 0b00000000, 99, 7, 0) == bitwise_copy_apply_test_state{0b00000001, 1}, "Test bitwise_copy_apply");
    static_assert(bitwise_copy_apply_test(0b11111111, 0b00000000, 99, 0, 7) == bitwise_copy_apply_test_state{0b10000000, 1}, "Test bitwise_copy_apply");
    static_assert(bitwise_copy_apply_test(0b01100011, 0b00000000, 1, 0, 0) == bitwise_copy_apply_test_state{0b00000001, 1}, "Test bitwise_copy_apply");
    static_assert(bitwise_copy_apply_test(0b01100011, 0b00000000, 3, 1, 0) == bitwise_copy_apply_test_state{0b00000001, 3}, "Test bitwise_copy_apply");
    static_assert(bitwise_copy_apply_test(0b01100011, 0b00000000, 1, 4, 0) == bitwise_copy_apply_test_state{0b00000000, 1}, "Test bitwise_copy_apply");

#pragma message("TODO: More test cases")

    /* 
     * atl
     * bit string definition
     */
#pragma message("TODO: Move the backing buffer stuff out into its own file, make utility functions for auto wrapping regions, make a region that auto reallocs with geometric growth, plus a utility function to get the atl::region from a backing buffer, IE {begin,ptr}")
    using bit_string_byte_type = unsigned char;

    struct backing_buffer_reserve_bytes_result_type
    {
        bit_string_byte_type* ptr;
        unsigned size;
    };

    template <typename backing_buffer_type>
    struct bit_string_buffer_wrapper_type
    {
        backing_buffer_type backing_buffer;
        bit_string_byte_type* current_byte;
        unsigned bit_offset;

        bit_string_buffer_wrapper_type(const backing_buffer_type& in_backing_buffer, unsigned in_bit_offset = 0)
            :
            backing_buffer(in_backing_buffer),
            current_byte(nullptr),
            bit_offset(in_bit_offset)
        {}
    };

    template <typename backing_buffer_type>
    bit_string_buffer_wrapper_type<backing_buffer_type> bit_string_wrap_backing_buffer(const backing_buffer_type& backing_buffer, unsigned in_bit_offset = 0)
    {
        return bit_string_buffer_wrapper_type<backing_buffer_type>(backing_buffer, in_bit_offset);
    }

    /* 
     * atl
     * bit string functions
     */    
    template <typename input_backing_buffer_type, typename output_backing_buffer_type>
    bool bit_string_copy_bits(bit_string_buffer_wrapper_type<input_backing_buffer_type>& input_buffer, bit_string_buffer_wrapper_type<output_backing_buffer_type>& output_buffer, const size_t total_bits_to_copy)
    {
        auto total_bits_copied = (unsigned)0;
        while(total_bits_copied < total_bits_to_copy)
        {
            const auto remaining_bits = total_bits_to_copy - total_bits_copied;
            const auto remaining_bytes = remaining_bits / 8;
            // Write byte-aligned data of largest possible size:
            if(remaining_bytes > 0 && input_buffer.bit_offset == 0 && output_buffer.bit_offset == 0)
            {
                const auto available_input_bytes = input_buffer.backing_buffer.reserve_bytes(remaining_bytes);
                const auto available_output_bytes = output_buffer.backing_buffer.reserve_bytes(available_input_bytes.size);
                const auto copyable_bytes = available_output_bytes.size;
                if(copyable_bytes > 0)
                {
                    std::copy(available_input_bytes.ptr, available_input_bytes.ptr + copyable_bytes, available_output_bytes.ptr);
                    total_bits_copied += copyable_bytes * 8;
                    input_buffer.backing_buffer.advance(copyable_bytes);
                    output_buffer.backing_buffer.advance(copyable_bytes);
                    continue;
                }
            }

            // Acquire 1 byte from each buffer
            // Early out with false if buffers have run out
            bit_string_byte_type* input_byte = input_buffer.bit_offset == 0 ? input_buffer.backing_buffer.reserve_bytes(1).ptr : input_buffer.current_byte;
            if(input_byte == nullptr) return false;

            bit_string_byte_type* output_byte;
            if(output_buffer.bit_offset == 0)
            {
                output_byte = output_buffer.backing_buffer.reserve_bytes(1).ptr;
                *output_byte = 0;
            }
            else output_byte = output_buffer.current_byte;
            if(output_byte == nullptr) return false;
            
            auto bits_copied = bitwise_copy_apply(*input_byte, *output_byte, remaining_bits, input_buffer.bit_offset, output_buffer.bit_offset);

            // Return false if writing was unsuccessful, as state will not advance
            if(bits_copied == 0) return false;

            // Acquire the bytes from input & output (IE, save the byte for next call, and advance the backing store)
            if(input_buffer.bit_offset == 0)
            {
                input_buffer.current_byte = input_byte;
                input_buffer.backing_buffer.advance(1);
            }
            if(output_buffer.bit_offset == 0)
            {
                output_buffer.current_byte = output_byte;
                output_buffer.backing_buffer.advance(1);
            }

            // Advance bit pointer:
            input_buffer.bit_offset = (input_buffer.bit_offset + bits_copied) % 8;
            output_buffer.bit_offset = (output_buffer.bit_offset + bits_copied) % 8;
            total_bits_copied += bits_copied;
        }
        return true;
    }

    template <typename input_backing_buffer_type, typename output_backing_buffer_type>
    bool bit_string_copy_bytes(bit_string_buffer_wrapper_type<input_backing_buffer_type>& input_buffer, bit_string_buffer_wrapper_type<output_backing_buffer_type>& output_buffer, uint32_t buffer_size_in_bytes)
    {
        return bit_string_copy_bits(input_buffer, output_buffer, buffer_size_in_bytes * 8);
    }

    /*
    High-level protocol:
    - Utility wrappers for serializaing & de-serializaing values conveniently
    - Specialized functions for things like "chunked integers" and "ranged integers"
    */

    struct simple_backing_buffer_type
    {
        bit_string_byte_type* begin, * end, * ptr;

        simple_backing_buffer_type(bit_string_byte_type* in_begin, size_t in_size)
        {
            ptr = begin = in_begin;
            end = begin + in_size;
        }
        unsigned remaining() const { return std::max(0, end - ptr); }
        unsigned most_available(unsigned num_bytes) const { return std::min(remaining(), num_bytes); }
        void advance(unsigned num_bytes) { ptr += most_available(num_bytes); }
        backing_buffer_reserve_bytes_result_type reserve_bytes(unsigned num_bytes)
        {
            auto available_bytes = most_available(num_bytes);
            return {available_bytes > 0 ? ptr : nullptr, available_bytes};
        }
    };

    inline simple_backing_buffer_type simple_backing_buffer(bit_string_byte_type* in_begin, size_t in_size)
    {
        return simple_backing_buffer_type(in_begin, in_size);
    }

    template <typename value_type>
    bit_string_buffer_wrapper_type<simple_backing_buffer_type> value_backing_buffer(value_type& value)
    {
        return bit_string_wrap_backing_buffer(simple_backing_buffer((bit_string_byte_type*)&value, sizeof(value)));
    }

    template <typename output_backing_buffer_type, typename value_type>
    bool bit_string_write_value(bit_string_buffer_wrapper_type<output_backing_buffer_type>& output_buffer, const value_type & input_value)
    {
        if constexpr(std::is_same<value_type, bool>::value)
        {
            const unsigned char copy_byte = (bool)input_value ? (const unsigned char)0xFF : (const unsigned char)0x00;
            return bit_string_copy_bits(value_backing_buffer(copy_byte), output_buffer, 1);
        }
        else return bit_string_copy_bytes(value_backing_buffer(input_value), output_buffer, sizeof(value_type));
    }

    template <typename input_backing_buffer_type, typename value_type>
    bool bit_string_read_value(bit_string_buffer_wrapper_type<input_backing_buffer_type>& input_buffer, value_type& output_value)
    {
        if constexpr(std::is_same<value_type, bool>::value)
        {
            unsigned char output_byte;
            if(!bit_string_copy_bits(input_buffer, value_backing_buffer(output_byte), 1)) return false;
            output_value = output_byte != 0;
            return true;
        }
        else return bit_string_copy_bytes(input_buffer, value_backing_buffer(output_value), sizeof(value_type));
    }
    
    template <typename integer_type, typename output_backing_buffer_type>
    bool bit_string_write_ranged_integer(bit_string_buffer_wrapper_type<output_backing_buffer_type>& output_buffer, const integer_type& value, const integer_type min, const integer_type max)
    {
        if(max == min) return true;
        else if(min > max) return false;
        value -= min;
        return bit_string_copy_bits(value_backing_buffer(value), output_buffer, bits_required_for_integer(max - min));
    }

    template <typename integer_type, typename input_backing_buffer_type>
    bool bit_string_read_ranged_integer(bit_string_buffer_wrapper_type<input_backing_buffer_type>& input_buffer, integer_type& value, const integer_type min, const integer_type max)
    {
        if(max == min)
        {
            value = min;
            return true;
        }
        else if(min > max) return false;
        auto delta = integer_type{0};
        if(bit_string_copy_bits(input_buffer, value_backing_buffer(delta), bits_required_for_integer(max - min)))
        {
            value = min + delta;
            return true;
        }
        else return false;
    }
    
    template <typename integer_type, typename output_backing_buffer_type>
    bool bit_string_write_chunked_integer(bit_string_buffer_wrapper_type<output_backing_buffer_type>& output_buffer, integer_type value, size_t chunk_size)
    {
        using local_integer_type = typename std::make_unsigned<integer_type>::type;
        constexpr local_integer_type local_integer_size_bits = sizeof(local_integer_type) * 8;
        local_integer_type current_value = std::is_signed<integer_type>::value ? ((value << 1) ^ (value >> (local_integer_size_bits - 1))) : value;
        local_integer_type mask = std::numeric_limits<local_integer_type>::max() >> (local_integer_size_bits - chunk_size);
        while(current_value > 0)
        {
            local_integer_type write_val = current_value & mask;
            if(!bit_string_write_value(output_buffer, true)) return false;
            if(!bit_string_copy_bits(value_backing_buffer(write_val), output_buffer, chunk_size)) return false;
            current_value = current_value >> chunk_size;
        }
        if(!bit_string_write_value(output_buffer, false)) return false;
        return true;
    }

    template <typename integer_type, typename input_backing_buffer_type>
    bool bit_string_read_chunked_integer(bit_string_buffer_wrapper_type<input_backing_buffer_type>& input_buffer, integer_type& value, size_t chunk_size)
    {
        integer_type result = 0;
        integer_type chunk;
        size_t current_bit = 0;
        bool chunk_flag;
        if(!bit_string_read_value(input_buffer, chunk_flag)) return false;
        while(chunk_flag)
        {
            chunk = 0;
            if(!bit_string_copy_bits(input_buffer, value_backing_buffer(chunk), chunk_size)) return false;
            result += chunk << current_bit;
            current_bit += chunk_size;
            chunk_flag = 0;
            if(!bit_string_read_value(input_buffer, chunk_flag)) return false;
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
    template <typename output_backing_buffer_type, typename value_type>
    bool bit_string_write_null_terminated_string(bit_string_buffer_wrapper_type<output_backing_buffer_type>& output_buffer, const value_type& input_value)
    {
        if(std::is_same<value_type, bool>::value)
        {
            const unsigned char copy_byte = (bool)input_value ? (const unsigned char)0xFF : (const unsigned char)0x00;
            return bit_string_copy_bits(value_backing_buffer(copy_byte), output_buffer, 1);
        }
        else return bit_string_copy_bytes(value_backing_buffer(input_value), output_buffer, sizeof(value_type));
    }

    template <typename input_backing_buffer_type, typename value_type>
    bool bit_string_read_null_terminated_string(bit_string_buffer_wrapper_type<input_backing_buffer_type>& input_buffer, value_type& output_value)
    {
        if(std::is_same<value_type, bool>::value)
        {
            unsigned char output_byte;
            if(!bit_string_copy_bits(input_buffer, value_backing_buffer(output_byte), 1)) return false;
            output_value = output_byte != 0;
            return true;
        }
        else return bit_string_copy_bytes(input_buffer, value_backing_buffer(output_value), sizeof(value_type));
    }
    */
}
