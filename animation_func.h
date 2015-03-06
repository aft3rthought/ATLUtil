#pragma once

#include <cstddef>
#include "ATLUtil/basic_math.h"

namespace atl
{
    class point2f;
    
    float ease_in_style1(const float in_val, const float in_bounce);
    float ease_in_style2(const float in_val);
    atl::point2f shake(const float in_magnitude, const float in_phase);
    
    template <typename KeyframeType>
    class keyframe {
    public:
        float t;
        KeyframeType v;
        
        keyframe(const float & in_t, const KeyframeType & in_v) : t(in_t), v(in_v) {};
        
        KeyframeType interpolateTo(float in_t, keyframe & in_next) {
            float l_delta = in_next.t - t;
            float l_pct = (in_t - t) / l_delta;
            auto l_result = v;
            l_result.interpolate(in_next.v, l_pct);
            return l_result;
        }
    };
    
    template <>
    class keyframe<float> {
    public:
        float t, v;
        
        float interpolateTo(float in_t, keyframe & in_next) {
            return atl::mapf_unclamped(in_t, t, in_next.t, v, in_next.v);
        }
    };
    
    template <typename KeyframeType, size_t N>
    KeyframeType map_keyframe(float t, keyframe<KeyframeType>(&keyframes)[N])
    {
        auto c = keyframes;
        auto end = c + N;
        if(t < c->t)
            return c->v;
        auto p = c++;
        while(c != end)
        {
            if(t < c->t)
                return p->interpolateTo(t, *c);
            p = c++;
        }
        return p->v;
    }
}