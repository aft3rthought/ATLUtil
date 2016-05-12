

#pragma once

#include "basic_math.h"
#include "math2d_fwd.h"
#include <cmath>
#include <algorithm>
#include <array>
#include <vector>

namespace atl
{
	namespace numbers {
		/*
#define M_LOG2E    1.44269504088896340736   // log2(e)
#define M_LOG10E   0.434294481903251827651  // log10(e)
#define M_LN2      0.693147180559945309417  // ln(2)
#define M_LN10     2.30258509299404568402   // ln(10)
#define M_1_PI     0.318309886183790671538  // 1/pi
#define M_2_PI     0.636619772367581343076  // 2/pi
#define M_2_SQRTPI 1.12837916709551257390   // 2/sqrt(pi)
#define M_SQRT1_2  0.707106781186547524401  // 1/sqrt(2)
*/
		static const float sqrt_2_f = 1.41421356237309504880f;
		static const float e_f = 2.71828182845904523536f;
		static const float two_pi_f = 2.f * 3.14159265358979323846f;
		static const float pi_f = 3.14159265358979323846f;
		static const float half_pi_f = 1.57079632679489661923f;
		static const float quarter_pi_f = 0.785398163397448309616f;
	}

    enum class anchoring
    {
        top_left,
        top_center,
        top_right,
        center_left,
        centered,
        center_right,
        bottom_left,
        bottom_center,
        bottom_right
    };
    
    class size2f
    {
    public:
        float w, h;
        
        size2f(float in_w, float in_h) : w(in_w), h(in_h) {};
        
        size2f(float in_val) : w(in_val), h(in_val) {};
        
        size2f() {};
        
        const static size2f Identity;
        
        size2f & operator *= (const float & in_scalar) {
            w *= in_scalar;
            h *= in_scalar;
            return *this;
        }
        
        size2f & operator /= (const float & in_scalar) {
            return *this *= (1.f / in_scalar);
        }

        size2f & operator += (const size2f & in_otherSize) {
            w += in_otherSize.w;
            h += in_otherSize.h;
            return *this;
        }
        
        size2f & operator -= (const size2f & in_otherSize) {
            w -= in_otherSize.w;
            h -= in_otherSize.h;
            return *this;
        }
        
        size2f operator * (const float & in_scalar) const {
            return size2f(*this) *= in_scalar;
        }
        
        size2f operator / (const float & in_scalar) const {
            return size2f(*this) /= in_scalar;
        }
        
        size2f operator + (const size2f & in_otherSize) const
        {
            return size2f(*this) += in_otherSize;
        }
        
        size2f operator - (const size2f & in_otherSize) const
        {
            return size2f(*this) -= in_otherSize;
        }
        
        bool operator ==(const size2f & in_otherSize) const {
            return w == in_otherSize.w && h == in_otherSize.h;
        }
        
        bool operator !=(const size2f & in_otherSize) const {
            return w != in_otherSize.w || h != in_otherSize.h;
        }
        
        size2f & interpolate(const size2f & in_to, float in_val)
        {
            w = interpf(w, in_to.w, in_val);
            h = interpf(h, in_to.h, in_val);
            return *this;
        }
    };

    class point2f
    {
    public:
        class vector_length
        {
        public:
            vector_length(float in_x, float in_y) :
            squared_value(in_x * in_x + in_y * in_y) {}
            
            bool operator <(float in_lengthInNonSquaredSpace) const {
                return squared_value < in_lengthInNonSquaredSpace * in_lengthInNonSquaredSpace;
            }
            
            bool operator >(float in_lengthInNonSquaredSpace) const {
                return squared_value > in_lengthInNonSquaredSpace * in_lengthInNonSquaredSpace;
            }
            
            bool operator <(const vector_length & in_otherLength) const {
                return squared_value < in_otherLength.squared_value;
            }
            
            bool operator >(const vector_length & in_otherLength) const {
                return squared_value > in_otherLength.squared_value;
            }
            
            float get_value() const {
                return sqrtf(squared_value);
            }
            
            static vector_length for_radius(float in_radius) {
                return {in_radius, 0.f};
            }
            
            float squared_value;
        };
        
        const static point2f AxisX;
        const static point2f AxisY;
        const static point2f Zero;

        float x, y;
        
        point2f(float in_x, float in_y) : x(in_x), y(in_y) {};
        point2f() {};
        
        point2f & set(float in_x, float in_y) {
            x = in_x;
            y = in_y;
            return *this;
        }
        
        bool operator == (const point2f & in_otherPoint) const {
            return x == in_otherPoint.x && y == in_otherPoint.y;
        }
        
        bool operator != (const point2f & in_otherPoint) const {
            return x != in_otherPoint.x || y != in_otherPoint.y;
        }
        
        point2f & interpolate(const point2f & in_to, float in_val)
        {
            x = interpf(x, in_to.x, in_val);
            y = interpf(y, in_to.y, in_val);
            return *this;
        }

        point2f operator - () const {
            return point2f(-x, -y);
        }
        
        vector_length length() const {
            return vector_length(x, y);
        }
        
        point2f & normalize() {
            return *this /= length().get_value();
        }
        
        point2f get_normal() const {
            return *this / length().get_value();
        };
        
        float dot(const point2f & in_otherPoint) const {
            return x * in_otherPoint.x + y * in_otherPoint.y;
        }
        
        float dot() const {
            return dot(*this);
        }
        
        point2f & operator += (const point2f & in_otherPoint) {
            x += in_otherPoint.x;
            y += in_otherPoint.y;
            return *this;
        }
        
        point2f & operator -= (const point2f & in_otherPoint) {
            x -= in_otherPoint.x;
            y -= in_otherPoint.y;
            return *this;
        }
        
        point2f & operator *= (float in_scalar) {
            x *= in_scalar;
            y *= in_scalar;
            return *this;
        }
        
        point2f & operator /= (float in_scalar) {
            return *this *= (1.f / in_scalar);
        }
        
        point2f operator + (const point2f & in_otherPt) const {
            return point2f(*this) += in_otherPt;
        }
        
        point2f operator - (const point2f & in_otherPt) const {
            return point2f(*this) -= in_otherPt;
        }
        
        point2f operator * (float in_scalar) const {
            return point2f(*this) *= in_scalar;
        }
        
        point2f operator / (float in_scalar) const {
            return point2f(*this) /= in_scalar;
        }
        
        static point2f get_normal_for_angle(float in_angle) {
            return point2f(cosf(in_angle), sinf(in_angle));
        }
        
        point2f get_projection(const point2f & in_otherPt) const {
            return point2f(*this) * dot(in_otherPt) / dot();
        }
        
        point2f get_cross_left() const {
            return point2f(-y, x);
        }
        
        point2f get_cross_right() const {
            return point2f(y, -x);
        }
        
        box2f get_box(const atl::size2f & in_size, anchoring in_anchoring);
    };

    class circlef
    {
    public:
        point2f center;
        float radius;
        
        circlef(float in_t, float in_r, float in_b, float in_l)
        {
            float l_width = in_r - in_l;
            float l_height = in_t - in_b;
            
            center.x = (in_r + in_l) * 0.5f;
            center.y = (in_b + in_t) * 0.5f;
            radius = (l_width + l_height) * 0.5f;
        }
        
        circlef(float in_centerX, float in_centerY, float in_radius) :
        center(in_centerX, in_centerY),
        radius(in_radius)
        {}
        
        circlef(const point2f & in_center, float in_radius) :
        center(in_center),
        radius(in_radius)
        {}
        
        point2f::vector_length distance(const point2f & in_point) const
        {
            return (in_point - center).length();
        }
        
        bool contains(const point2f & in_point) const
        {
            return distance(in_point) < point2f::vector_length::for_radius(radius);
        }
        
        bool overlaps(const circlef & in_otherCircle) const
        {
            float l_combined_radii = in_otherCircle.radius + radius;
            
            return distance(in_otherCircle.center) < point2f::vector_length::for_radius(l_combined_radii);
        }
    };
    
    class rangef
    {
    public:
        float min, max;
        
        rangef(float in_min, float in_max) :
        min(in_min),
        max(in_max)
        {}
        
        rangef()
        {}
        
        const static rangef Max;
        const static rangef InvertedMax;
        
        bool operator == (const rangef & in_otherRange) const {
            return min == in_otherRange.min && max == in_otherRange.max;
        }
        
        bool operator != (const rangef & in_otherRange) const {
            return min != in_otherRange.min || max != in_otherRange.max;
        }
        
        bool empty() const {
            return max <= min;
        }

        bool inverted() const {
            return max < min;
        }
        
        float length() const {
            return max - min;
        }
        
        float center() const {
            return (min + max) * 0.5f;
        }
        
        float pct(float in_t) const {
            return (max - min) * in_t + min;
        }
        
        bool contains(float in_value) const {
            return in_value >= min && in_value <= max;
        }

        bool contains(const rangef & in_otherRange) const {
            return in_otherRange.min >= min && in_otherRange.max <= max;
        }
        
        bool overlaps(const rangef & in_otherRange) const {
            return in_otherRange.min <= max && in_otherRange.max >= min;
        }
        
        rangef get_intersection(const rangef & in_otherRange) const {
            return {
                std::max(min, in_otherRange.min),
                std::min(max, in_otherRange.max)
            };
        }
        
        rangef & move_up(float in_offset) {
            min += in_offset;
            max += in_offset;
            return *this;
        }
        
        rangef & move_down(float in_offset) {
            min -= in_offset;
            max -= in_offset;
            return *this;
        }
        
        rangef & grow(float in_offset) {
            min -= in_offset;
            max += in_offset;
            return *this;
        }
        
        rangef & shrink(float in_offset) {
            min += in_offset;
            max -= in_offset;
            return *this;
        }
        
        rangef & scale(float in_scalar) {
            float l_center = (max + min) * 0.5f;
            float l_newHalfLength = (max - min) * in_scalar * 0.5f;
            min = l_center - l_newHalfLength;
            max = l_center + l_newHalfLength;
            return *this;
        }
        
        rangef & flip() {
            std::swap(min, max);
            return *this;
        }
        
        rangef & include(float in_value) {
            min = std::min(min, in_value);
            max = std::max(max, in_value);
            return *this;
        }

        rangef & include(const rangef & in_otherRange) {
            min = std::min(min, in_otherRange.min);
            max = std::max(max, in_otherRange.max);
            return *this;
        }
        
        rangef & interpolate(const rangef & in_to, float in_t) {
            min = interpf(min, in_to.min, in_t);
            max = interpf(max, in_to.max, in_t);
            return *this;
        }
    };
    
    class box2f
    {
    public:
        union {
            struct {
                float l, r, b, t;
            };
            struct {
                rangef x, y;
            };
        };
        
        box2f(const rangef & in_x, const rangef & in_y) :
        x(in_x),
        y(in_y)
        {}
        
        box2f(float in_t, float in_r, float in_b, float in_l) :
        t(in_t),
        r(in_r),
        b(in_b),
        l(in_l)
        {}

        box2f(const atl::point2f & in_pt) :
        t(in_pt.y),
        r(in_pt.x),
        b(in_pt.y),
        l(in_pt.x)
        {}
        
        template <typename Iterator>
        box2f(Iterator in_itr, Iterator in_end)
        {
            l = r = in_itr->x;
            b = t = in_itr->y;
            while(++in_itr != in_end)
            {
                l = std::min(l, in_itr->x);
                b = std::min(b, in_itr->y);
                r = std::max(r, in_itr->x);
                t = std::max(t, in_itr->y);
            }
        }
        
        box2f(const std::initializer_list<atl::point2f> & in_ptList) :
        box2f(in_ptList.begin(), in_ptList.end())
        {}

        box2f(const std::vector<atl::point2f> & in_ptList) :
        box2f(in_ptList.begin(), in_ptList.end())
        {}

        template <int N>
        box2f(const std::array<atl::point2f, N> & in_ptList) :
        box2f(in_ptList.begin(), in_ptList.end())
        {}
        
        box2f(const point2f & in_corner, const size2f & in_size, anchoring in_anchoring)
        {
            // X coordinate:
            switch(in_anchoring)
            {
                case anchoring::top_left:
                case anchoring::center_left:
                case anchoring::bottom_left:
                {
                    l = in_corner.x;
                    r = in_corner.x + in_size.w;
                    break;
                }
                    
                case anchoring::top_center:
                case anchoring::centered:
                case anchoring::bottom_center:
                {
                    float l_halfWidth = in_size.w / 2.f;
                    l = in_corner.x - l_halfWidth;
                    r = in_corner.x + l_halfWidth;
                    break;
                }
                    
                case anchoring::top_right:
                case anchoring::center_right:
                case anchoring::bottom_right:
                {
                    l = in_corner.x - in_size.w;
                    r = in_corner.x;
                    break;
                }
            }
            
            // Y coordinate:
            switch(in_anchoring)
            {
                case anchoring::top_left:
                case anchoring::top_center:
                case anchoring::top_right:
                {
                    b = in_corner.y - in_size.h;
                    t = in_corner.y;
                    break;
                }
                    
                case anchoring::center_left:
                case anchoring::centered:
                case anchoring::center_right:
                {
                    float l_halfHeight = in_size.h / 2.f;
                    b = in_corner.y - l_halfHeight;
                    t = in_corner.y + l_halfHeight;
                    break;
                }
                    
                case anchoring::bottom_left:
                case anchoring::bottom_center:
                case anchoring::bottom_right:
                {
                    b = in_corner.y;
                    t = in_corner.y + in_size.h;
                    break;
                }
            }
        }
        
        box2f(circlef in_circle) :
        t(in_circle.center.y + in_circle.radius),
        r(in_circle.center.x + in_circle.radius),
        b(in_circle.center.y - in_circle.radius),
        l(in_circle.center.x - in_circle.radius)
        {}
        
        box2f()
        {}
        
        const static box2f MaxInvertedBounds;
        const static box2f MaxBounds;
        
        float width() const { return x.length(); }
        float height() const { return y.length(); }
        size2f size() const { return size2f(width(), height()); }
        
        point2f center() const { return point2f(x.center(), y.center()); }
        point2f bottom_left() const { return point2f(l, b); }
        point2f top_left() const { return point2f(l, t); }
        point2f top_right() const { return point2f(r, t); }
        point2f bottom_right() const { return point2f(r, b); }
        point2f center_left() const { return point2f(l, y.center()); }
        point2f top_center() const { return point2f(x.center(), t); }
        point2f center_right() const { return point2f(r, y.center()); }
        point2f bottom_center() const { return point2f(x.center(), b); }

        bool inverted() const {
            return x.inverted() || y.inverted();
        }
        
        bool empty() const {
            return x.empty() || y.empty();
        }
        
        box2f & set(float in_t, float in_r, float in_b, float in_l) {
            t = in_t;
            r = in_r;
            b = in_b;
            l = in_l;
            return *this;
        }
        
        /*
         * grow/shrink by a float
         */
        box2f & operator += (float in_offset) {
            x.grow(in_offset);
            y.grow(in_offset);
            return *this;
        }
        
        box2f & operator -= (float in_offset) {
            x.shrink(in_offset);
            y.shrink(in_offset);
            return *this;
        }
        
        box2f operator + (float in_offset) const {
            return box2f(*this) += in_offset;
        }
        
        box2f operator - (float in_offset) const {
            return box2f(*this) -= in_offset;
        }
        
        /*
         * translate/untranslate by a point
         */
        box2f & operator += (const point2f & in_pt) {
            x.move_up(in_pt.x);
            y.move_up(in_pt.y);
            return *this;
        }
        
        box2f & operator -= (const point2f & in_pt) {
            x.move_down(in_pt.x);
            y.move_down(in_pt.y);
            return *this;
        }
        
        box2f operator + (const point2f & in_pt) const {
            return box2f(*this) += in_pt;
        }
        
        box2f operator - (const point2f & in_pt) const {
            return box2f(*this) -= in_pt;
        }
        
        /*
         * grow/shrink by a percent specified by a size
         */
        box2f & operator *= (const size2f & in_size) {
            x.scale(in_size.w);
            y.scale(in_size.h);
            return *this;
        }
        
        box2f operator * (const size2f & in_size) const {
            return box2f(*this) *= in_size;
        }
        
        /*
         * range functions
         */
        box2f & include(const point2f & in_point) {
            x.include(in_point.x);
            y.include(in_point.y);
            return *this;
        }
        
        box2f & include(const box2f & in_otherBounds) {
            x.include(in_otherBounds.x);
            y.include(in_otherBounds.y);
            return *this;
        }
        
        box2f get_intersection(const box2f & in_otherBounds) const {
            return {
                x.get_intersection(in_otherBounds.x),
                y.get_intersection(in_otherBounds.y)
            };
        }
        
        bool operator ==(const box2f & in_otherBounds) const {
            return x == in_otherBounds.x && y == in_otherBounds.y;
        }
        
        bool operator !=(const box2f & in_otherBounds) const {
            return x != in_otherBounds.x || y != in_otherBounds.y;
        }
        
        /*
         * constructions
         */
        box2f get_flipx() const {
            return { rangef(x).flip(), y };
        }

        box2f get_flipy() const {
            return { x, rangef(y).flip() };
        }
        
        box2f & anchor_in_parent(const box2f & in_parent, anchoring in_anchoring) {
            box2f l_result = in_parent.get_sub_bounds_with_size(size(), in_anchoring);
            t = l_result.t;
            r = l_result.r;
            b = l_result.b;
            l = l_result.l;
            return *this;
        }
        
        box2f get_sub_bounds_with_size(const size2f & in_size, anchoring in_anchoring) const
        {
            box2f l_result;
            switch(in_anchoring)
            {
                case anchoring::top_left:
                case anchoring::center_left:
                case anchoring::bottom_left:
                    l_result.l = l;
                    l_result.r = l + in_size.w;
                    break;
                case anchoring::top_center:
                case anchoring::centered:
                case anchoring::bottom_center:
                    l_result.l = (l + r - in_size.w) / 2.f;
                    l_result.r = (l + r + in_size.w) / 2.f;
                    break;
                case anchoring::top_right:
                case anchoring::center_right:
                case anchoring::bottom_right:
                    l_result.r = r;
                    l_result.l = r - in_size.w;
                    break;
            }
            
            switch(in_anchoring)
            {
                case anchoring::top_left:
                case anchoring::top_center:
                case anchoring::top_right:
                    l_result.t = t;
                    l_result.b = t - in_size.h;
                    break;
                case anchoring::center_left:
                case anchoring::centered:
                case anchoring::center_right:
                    l_result.b = (b + t - in_size.h) / 2.f;
                    l_result.t = (b + t + in_size.h) / 2.f;
                    break;
                case anchoring::bottom_left:
                case anchoring::bottom_center:
                case anchoring::bottom_right:
                    l_result.b = b;
                    l_result.t = b + in_size.h;
                    break;
            }
            
            return l_result;
        }
        
        bool contains(const point2f & in_p) const
        {
            return x.contains(in_p.x) && y.contains(in_p.y);
        }
        
        bool touches(const box2f & in_otherBounds) const
        {
            return x.overlaps(in_otherBounds.x) && y.overlaps(in_otherBounds.y);
        }
        
        bool contains(const box2f & in_otherBounds) const
        {
            return x.contains(in_otherBounds.x) && y.contains(in_otherBounds.y);
        }
        
        box2f & interpolate(const box2f & in_to, float in_t)
        {
            x.interpolate(in_to.x, in_t);
            y.interpolate(in_to.y, in_t);
            return *this;
        }
        
        box2f split_right(float in_ratio)
        {
            float l_mid_point = l + (r - l) * in_ratio;
            box2f l_result(t, r, b, l_mid_point);
            r = l_mid_point;
            return l_result;
        }
        
        box2f split_down(float in_ratio)
        {
            float l_mid_point = t + (b - t) * in_ratio;
            box2f l_result(l_mid_point, r, b, l);
            b = l_mid_point;
            return l_result;
        }
    };
    
    class bounds4i
    {
    public:
        int32_t t, r, b, l;
        
        bounds4i(int32_t in_t,
                 int32_t in_r,
                 int32_t in_b,
                 int32_t in_l) :
        t(in_t),
        r(in_r),
        b(in_b),
        l(in_l)
        {}
        
        int32_t width() const { return r - l; }
        int32_t height() const { return t - b; }
        
        const static bounds4i EmptyGrowableBounds;
        
        void extrude(int32_t in_amount)
        {
            t += in_amount;
            r += in_amount;
            b -= in_amount;
            l -= in_amount;
        }
        
        enum class Contains {
            Outside,
            Edge,
            Inside
        };
        
        bool touches(const bounds4i & in_otherBounds) const {
            if(l > in_otherBounds.r ||
               r < in_otherBounds.l ||
               b > in_otherBounds.t ||
               t < in_otherBounds.b)
                return false;
            return true;
        };
        
        Contains contains(int32_t in_x, int32_t in_y) const
        {
            if(in_x < l ||
               in_y < b ||
               in_x > r ||
               in_y > t)
                return Contains::Outside;
            return Contains::Inside;
        }
       
        Contains contains(const bounds4i & in_otherBounds) const
        {
            if(l > in_otherBounds.l &&
               r < in_otherBounds.r &&
               b > in_otherBounds.b &&
               t < in_otherBounds.t)
                return Contains::Inside;
            return Contains::Outside;
        }
        
        void set(int32_t in_t, int32_t in_r, int32_t in_b, int32_t in_l) {
            t = in_t;
            r = in_r;
            b = in_b;
            l = in_l;
        }
        
        void translate(int32_t in_x, int32_t in_y) {
            t += in_y;
            r += in_x;
            b += in_y;
            l += in_x;
        }
        
        void untranslate(int32_t in_x, int32_t in_y) {
            t -= in_y;
            r -= in_x;
            b -= in_y;
            l -= in_x;
        }
        
        void include(int32_t in_ptX, int32_t in_ptY) {
            t = std::max(t, in_ptY);
            r = std::max(r, in_ptX);
            b = std::min(b, in_ptY);
            l = std::min(l, in_ptX);
        }
        
        void include(const bounds4i & in_otherBounds) {
            t = std::max(t, in_otherBounds.t);
            r = std::max(r, in_otherBounds.r);
            b = std::min(b, in_otherBounds.b);
            l = std::min(l, in_otherBounds.l);
        }
        
        bounds4i get_intersection(const bounds4i & in_otherBounds) const {
            bounds4i result(std::min(t, in_otherBounds.t),
                            std::min(r, in_otherBounds.r),
                            std::max(b, in_otherBounds.b),
                            std::max(l, in_otherBounds.l));
            return result;
        }
        
        bool operator ==(const bounds4i & in_otherBounds) const {
            return (t == in_otherBounds.t &&
                    r == in_otherBounds.r &&
                    b == in_otherBounds.b &&
                    l == in_otherBounds.l);
        }
        
        bool operator !=(const bounds4i & in_otherBounds) const {
            return (t != in_otherBounds.t ||
                    r != in_otherBounds.r ||
                    b != in_otherBounds.b ||
                    l != in_otherBounds.l);
        }
    };
    
    inline box2f point2f::get_box(const atl::size2f & in_size, anchoring in_anchoring)
    {
        box2f l_result;
        switch(in_anchoring)
        {
            case anchoring::top_left:
            case anchoring::center_left:
            case anchoring::bottom_left:
            {
                l_result.l = x;
                l_result.r = x + in_size.w;
                break;
            }
            case anchoring::top_center:
            case anchoring::centered:
            case anchoring::bottom_center:
            {
                float l_w = in_size.w * 0.5f;
                l_result.l = x - l_w;
                l_result.r = x + l_w;
                break;
            }
            case anchoring::top_right:
            case anchoring::center_right:
            case anchoring::bottom_right:
            {
                l_result.r = x;
                l_result.l = x - in_size.w;
                break;
            }
        }
        switch(in_anchoring)
        {
            case anchoring::top_left:
            case anchoring::top_center:
            case anchoring::top_right:
                l_result.t = y;
                l_result.b = y - in_size.h;
                break;
            case anchoring::center_left:
            case anchoring::centered:
            case anchoring::center_right:
            {
                float l_h = in_size.h * 0.5f;
                l_result.b = y - l_h;
                l_result.t = y + l_h;
                break;
            }
            case anchoring::bottom_left:
            case anchoring::bottom_center:
            case anchoring::bottom_right:
                l_result.b = y;
                l_result.t = y + in_size.h;
                break;
        }
        return l_result;
    }
}