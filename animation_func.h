#pragma once

namespace atl
{
    class point2f;
    
    float ease_in_style1(const float in_val, const float in_bounce);
    float ease_in_style2(const float in_val);
    atl::point2f shake(const float in_magnitude, const float in_phase);
}