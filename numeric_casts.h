

#pragma once

namespace atl
{
    constexpr uint32_t narrow_unsigned(uint64_t val) { return (uint32_t)val; }
    constexpr int32_t narrow_signed(int64_t val) { return (int32_t)val; }
    constexpr int32_t make_signed(uint32_t val) { return (int32_t)val; }
    constexpr int32_t narrow_size_t_to_int(size_t val) { return (int32_t)val; }
}
