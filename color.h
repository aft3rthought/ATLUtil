

#pragma once

#include "basic_math.h"

namespace atl
{
    class color_alpha
    {
    public:
        float alpha;
        
        color_alpha(float in_alpha) :
        alpha(in_alpha)
        {}
    };
    
    class color_monochrome
    {
    public:
        float value;
        float alpha;
        
        color_monochrome(float in_value) :
        value(in_value),
        alpha(1.f)
        {}
        
        color_monochrome(float in_value, float in_alpha) :
        value(in_value),
        alpha(in_alpha)
        {}
    };
    
    class color
    {
    public:
        float r;
        float g;
        float b;
        float a;
        
        color(float in_r, float in_g, float in_b) :
        r(in_r),
        g(in_g),
        b(in_b),
        a(1.f)
        {};
        
        color(float in_r, float in_g, float in_b, float in_a) :
        r(in_r),
        g(in_g),
        b(in_b),
        a(in_a)
        {};
        
        color(const color_monochrome & in_color) :
        r(in_color.value),
        g(in_color.value),
        b(in_color.value),
        a(in_color.alpha)
        {};
        
        color(const color_alpha & in_color) :
        r(1.f),
        g(1.f),
        b(1.f),
        a(in_color.alpha)
        {};
        
        color(unsigned char in_r, unsigned char in_g, unsigned char in_b) :
        r(float(in_r) / 255.f),
        g(float(in_g) / 255.f),
        b(float(in_b) / 255.f),
        a(1.f)
        {};
        
        color(unsigned char in_r, unsigned char in_g, unsigned char in_b, unsigned char in_a) :
        r(float(in_r) / 255.f),
        g(float(in_g) / 255.f),
        b(float(in_b) / 255.f),
        a(float(in_a) / 255.f)
        {};
        
        void set(float in_r, float in_g, float in_b, float in_a)
        {
            r = in_r;
            g = in_g;
            b = in_b;
            a = in_a;
        };
        
        bool operator ==(const color & in_otherPoint) {
            return (r == in_otherPoint.r &&
                    g == in_otherPoint.g &&
                    b == in_otherPoint.b &&
                    a == in_otherPoint.a);
        }
        
        bool operator !=(const color & in_otherPoint) {
            return !((*this) == in_otherPoint);
        }
        
        void interpolate(const color & in_to, float in_val)
        {
            r = interpf(r, in_to.r, in_val);
            g = interpf(g, in_to.g, in_val);
            b = interpf(b, in_to.b, in_val);
            a = interpf(a, in_to.a, in_val);
        }
    };
    
    class color_premul
    {
    public:
        float r;
        float g;
        float b;
        float a;
        
        color_premul(unsigned char in_r, unsigned char in_g, unsigned char in_b, unsigned char in_a) :
        r(float(in_r) / 255.f),
        g(float(in_g) / 255.f),
        b(float(in_b) / 255.f),
        a(float(in_a) / 255.f)
        {};
        
        color_premul(float in_r, float in_g, float in_b, float in_a) :
        r(in_r),
        g(in_g),
        b(in_b),
        a(in_a)
        {}
        
        color_premul(const color & in_color) :
        r(in_color.r * in_color.a),
        g(in_color.g * in_color.a),
        b(in_color.b * in_color.a),
        a(in_color.a)
        {}
        
        color_premul(const color_monochrome & in_color) :
        r(in_color.value * in_color.alpha),
        g(in_color.value * in_color.alpha),
        b(in_color.value * in_color.alpha),
        a(in_color.alpha)
        {}
        
        color_premul(const color_alpha & in_color) :
        r(in_color.alpha),
        g(in_color.alpha),
        b(in_color.alpha),
        a(in_color.alpha)
        {}
        
        void set(float in_r, float in_g, float in_b, float in_a) {
            r = in_r;
            g = in_g;
            b = in_b;
            a = in_a;
        }
        
        bool operator ==(const color_premul & in_otherColor) {
            return (r == in_otherColor.r &&
                    g == in_otherColor.g &&
                    b == in_otherColor.b &&
                    a == in_otherColor.a);
        }
        
        bool operator !=(const color_premul & in_otherColor) {
            return !((*this) == in_otherColor);
        }
    };
}