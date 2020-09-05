

#include "animation_func.h"
#include "math2d.h"
#include <cmath>

float atl::ease_in_style1(const float in_val, const float in_bounce)
{
    return in_val * in_val + std::sinf(in_val * in_val * atl::numbers::pi_f) * in_bounce;
}

float atl::ease_in_style2(const float in_val)
{
	auto cos = std::cosf(in_val * atl::numbers::half_pi_f);
    return 1.f - (cos * cos * cos * cos);
}

float atl::ease_in_sine(const float in_val)
{
	return 1.f - std::cosf(in_val * atl::numbers::half_pi_f);
}

float atl::ease_out_sine(const float in_val)
{
	return std::cosf((1.f - in_val) * atl::numbers::half_pi_f);
}

float atl::ease_in_out_sine(const float in_val)
{
	return (std::cosf((1.f + in_val) * atl::numbers::pi_f) + 1.f) / 2.f;
}

float atl::ease_in_style2_integral(const float in_val)
{
	auto sin = std::sinf(atl::numbers::pi_f * in_val);
	auto cos = std::cosf(atl::numbers::pi_f * in_val);
	return (5.f * in_val) / 8.f - sin / atl::numbers::two_pi_f - (sin * cos) / atl::numbers::eight_pi_f;
}

atl::point2f atl::shake(const float in_magnitude, const float in_phase)
{
	auto sine_val = std::sinf(in_phase);
	auto cos_val = std::cosf(in_phase);
	auto x_amount = in_magnitude * sine_val;
	auto y_amount = in_magnitude * sine_val;
    return atl::point2f(sine_val * x_amount, cos_val * y_amount);
}
