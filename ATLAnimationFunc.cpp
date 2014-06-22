

#include "ATLAnimationFunc.h"
#include <cmath>

float atl::ease_in_style1(const float in_val, const float in_bounce)
{
    return in_val * in_val + sinf(in_val * in_val * M_PI) * in_bounce;
}

float atl::ease_in_style2(const float in_val)
{
    float cos = cosf(in_val * M_PI / 2.f);
    return 1.f - (cos * cos * cos * cos);
}