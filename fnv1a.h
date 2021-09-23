#pragma once

#include <cstdint>

// 64 bit fnv1a implementation
const uint64_t fnv1a_prime = 1099511628211u;
const uint64_t fnv1a_seed = 14695981039346656037u;

constexpr uint64_t fnv1a_byte(unsigned char input_byte, uint64_t hash_value = fnv1a_seed)
{
	return (input_byte ^ hash_value) * fnv1a_prime;
}

constexpr uint64_t fnv1a_compile_time_string(const char* string, uint64_t hash_value = fnv1a_seed)
{
	return (*string == 0) ? hash_value : fnv1a_compile_time_string(string + 1, fnv1a_byte(*string, hash_value));
}

template <typename large_type>
inline uint64_t fnv1a_any(const large_type& input_byte, uint64_t hash_value = fnv1a_seed)
{
	auto data_ptr = static_cast<const unsigned char*>(static_cast<const void*>(&input_byte));
	const auto data_end = data_ptr + sizeof(large_type);
	for(; data_ptr != data_end; ++data_ptr) hash_value = fnv1a_byte(*data_ptr, hash_value);
	return hash_value;
}
