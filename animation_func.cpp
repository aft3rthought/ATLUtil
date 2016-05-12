

#include "animation_func.h"
#include "math2d.h"
#include <cmath>

float atl::ease_in_style1(const float in_val, const float in_bounce)
{
    return in_val * in_val + sinf(in_val * in_val * atl::numbers::pi_f) * in_bounce;
}

float atl::ease_in_style2(const float in_val)
{
    float cos = cosf(in_val * atl::numbers::half_pi_f);
    return 1.f - (cos * cos * cos * cos);
}

atl::point2f atl::shake(const float in_magnitude, const float in_phase)
{
    float sineVal = sinf(in_phase);
    float xAmount = in_magnitude * sineVal;
    float yAmount = in_magnitude * sineVal;
    return atl::point2f(sineVal * xAmount, cosf(in_phase) * yAmount);
}
