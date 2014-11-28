

#pragma once

#include "basic_math.h"
#include <cmath>
#include <algorithm>

namespace atl
{
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
        
        size2f operator *(const float & in_scalar) const {
            return size2f(w * in_scalar, h * in_scalar);
        }
        
        size2f & operator *=(const float & in_scalar) {
            w *= in_scalar;
            h *= in_scalar;
            return *this;
        }
        
        size2f operator /(const float & in_scalar) const {
            return size2f(w / in_scalar, h / in_scalar);
        }
        
        size2f & operator /=(const float & in_scalar) {
            w /= in_scalar;
            h /= in_scalar;
            return *this;
        }
        
        size2f operator + (const size2f & in_otherSize) const
        {
            return size2f(w + in_otherSize.w, h + in_otherSize.h);
        }
        
        size2f operator - (const size2f & in_otherSize) const
        {
            return size2f(w - in_otherSize.w, h - in_otherSize.h);
        }
        
        bool operator ==(const size2f & in_otherSize) const {
            return w == in_otherSize.w && h == in_otherSize.h;
        }
        
        bool operator !=(const size2f & in_otherSize) const {
            return w != in_otherSize.w || h != in_otherSize.h;
        }
        
        void interpolate(const size2f& in_to, float in_val)
        {
            w = interpf(w, in_to.w, in_val);
            h = interpf(h, in_to.h, in_val);
        }
    };

    class point2f
    {
    public:
        class vecLength
        {
        public:
            vecLength(float in_dist) : _squaredValue(in_dist * in_dist) {}
            
            vecLength(float in_vA, float in_vB) : _squaredValue(in_vA * in_vA + in_vB * in_vB) {}
            
            bool operator <(const vecLength & in_otherLength) const {
                return _squaredValue < in_otherLength._squaredValue;
            }
            
            bool operator >(const vecLength & in_otherLength) const {
                return _squaredValue > in_otherLength._squaredValue;
            }
            
            float getValue() const {
                return sqrtf(_squaredValue);
            }
            
        private:
            float _squaredValue;
        };
        
        float x, y;
        
        point2f(float in_x, float in_y) : x(in_x), y(in_y) {};
        point2f() {};
        
        void set(float in_x, float in_y) {
            x = in_x;
            y = in_y;
        }
        
        bool operator == (const point2f & in_otherPoint) const {
            return x == in_otherPoint.x && y == in_otherPoint.y;
        }
        
        bool operator != (const point2f & in_otherPoint) const {
            return x != in_otherPoint.x || y != in_otherPoint.y;
        }
        
        void interpolate(const point2f & in_to, float in_val)
        {
            x = interpf(x, in_to.x, in_val);
            y = interpf(y, in_to.y, in_val);
        }
        
        point2f & operator += (const point2f & in_vec) {
            x += in_vec.x;
            y += in_vec.y;
            return *this;
        }
        
        point2f & operator -= (const point2f & in_vec) {
            x -= in_vec.x;
            y -= in_vec.y;
            return *this;
        }

        point2f & operator - () {
            x = -x;
            y = -y;
            return *this;
        }
        
        vecLength length() const {
            return vecLength(x, y);
        }
        
        void normalize() {
            float currentLength = length().getValue();
            x = x / currentLength;
            y = y / currentLength;
        }
        
        float dot() const {
            return x * x + y * y;
        }

        float dot(const point2f & in_other) const {
            return x * in_other.x + y * in_other.y;
        }
        
        point2f getNormal() const {
            float currentLength = length().getValue();
            return point2f(x / currentLength, y / currentLength);
        };
        
        point2f getCross() const {
            return point2f(-y, x);
        };
        
        bool operator *= (const point2f & in_other) {
            return x != in_other.x || y != in_other.y;
        }
        
        point2f & operator *= (float in_scalar) {
            x *= in_scalar;
            y *= in_scalar;
            return *this;
        }
        
        point2f & operator /= (float in_scalar) {
            x /= in_scalar;
            y /= in_scalar;
            return *this;
        }
        
        point2f operator * (float in_scalar) const {
            return point2f(x * in_scalar, y * in_scalar);
        }
        
        point2f operator / (float in_scalar) const {
            return point2f(x / in_scalar, y / in_scalar);
        }
        
        static point2f ForAngle(float in_angle)
        {
            return point2f(cosf(in_angle), sinf(in_angle));
        }
      
        point2f operator + (const point2f & in_otherPt) const
        {
            return point2f(x + in_otherPt.x, y + in_otherPt.y);
        }
        
        point2f operator - (const point2f & in_otherPt) const
        {
            return point2f(x - in_otherPt.x, y - in_otherPt.y);
        }
    };

    /*
     atl::circlef
     */
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
        
        point2f::vecLength distance(const point2f & in_point) const
        {
            return (in_point - center).length();
        }
        
        bool contains(const point2f & in_point) const
        {
            return distance(in_point) < point2f::vecLength(radius);
        }
        
        bool overlaps(const circlef & in_otherCircle) const
        {
            float combinedRadii = in_otherCircle.radius + radius;
            
            return distance(in_otherCircle.center) < point2f::vecLength(combinedRadii);
        }
    };
    
    class bounds4f
    {
    public:
        float t, r, b, l;
        
        bounds4f(float in_t, float in_r, float in_b, float in_l) :
        t(in_t),
        r(in_r),
        b(in_b),
        l(in_l)
        {}
        
        bounds4f(const point2f & in_corner, const size2f & in_size, anchoring in_anchoring)
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
        
        bounds4f()
        {}
        
        bounds4f(circlef in_circle) :
        t(in_circle.center.y + in_circle.radius),
        r(in_circle.center.x + in_circle.radius),
        b(in_circle.center.y - in_circle.radius),
        l(in_circle.center.x - in_circle.radius)
        {}
        
        const static bounds4f EmptyGrowableBounds;
        
        float centerx() const { return (l + r) / 2.f; }
        float centery() const { return (t + b) / 2.f; }
        
        float pctx(float in_pct) const { return l + (r - l) * in_pct; }
        float pcty(float in_pct) const { return b + (t - b) * in_pct; }
        
        float width() const { return r - l; }
        float height() const { return t - b; }
        
        point2f center() const { return point2f(centerx(), centery()); }
        point2f bottom_left() const { return point2f(l, b); }
        point2f top_left() const { return point2f(l, t); }
        point2f top_right() const { return point2f(r, t); }
        point2f bottom_right() const { return point2f(r, b); }
        size2f size() const { return size2f(width(), height()); }
        
        bool empty() const {
            return t <= b || r <= l;
        }
        
        void set(float in_t, float in_r, float in_b, float in_l) {
            t = in_t;
            r = in_r;
            b = in_b;
            l = in_l;
        }
        
        void translate(float in_x, float in_y) {
            t += in_y;
            r += in_x;
            b += in_y;
            l += in_x;
        }
        
        void translate(const point2f & in_vec) {
            translate(in_vec.x, in_vec.y);
        }
        
        void untranslate(float in_x, float in_y) {
            t -= in_y;
            r -= in_x;
            b -= in_y;
            l -= in_x;
        }
        
        void untranslate(const point2f & in_vec) {
            untranslate(in_vec.x, in_vec.y);
        }
        
        void include(const point2f & in_point) {
            t = std::max(t, in_point.y);
            r = std::max(r, in_point.x);
            b = std::min(b, in_point.y);
            l = std::min(l, in_point.x);
        }
        
        void include(const bounds4f & in_otherBounds) {
            t = std::max(t, in_otherBounds.t);
            r = std::max(r, in_otherBounds.r);
            b = std::min(b, in_otherBounds.b);
            l = std::min(l, in_otherBounds.l);
        }
        
        bounds4f get_intersection(const bounds4f & in_otherBounds) const {
            bounds4f result(std::min(t, in_otherBounds.t),
                            std::min(r, in_otherBounds.r),
                            std::max(b, in_otherBounds.b),
                            std::max(l, in_otherBounds.l));
            return result;
        }
        
        bool operator ==(const bounds4f & in_otherBounds) const {
            return (t == in_otherBounds.t &&
                    r == in_otherBounds.r &&
                    b == in_otherBounds.b &&
                    l == in_otherBounds.l);
        }
        
        bool operator !=(const bounds4f & in_otherBounds) const {
            return (t != in_otherBounds.t ||
                    r != in_otherBounds.r ||
                    b != in_otherBounds.b ||
                    l != in_otherBounds.l);
        }
        
        void scale_width_about_center(float in_scalar) {
            float l_halfWidth = width() / 2.f;
            float l_centerX = l + l_halfWidth;
            
            l_halfWidth *= in_scalar;
            
            l = l_centerX - l_halfWidth;
            r = l_centerX + l_halfWidth;
        }
        
        void extrude_width(float in_amount) {
            l -= in_amount;
            r += in_amount;
        }
        
        void scale_height_about_center(float in_scalar) {
            float l_halfHeight = height() / 2.f;
            float l_centerY = b + l_halfHeight;
            
            l_halfHeight *= in_scalar;
            
            b = l_centerY - l_halfHeight;
            t = l_centerY + l_halfHeight;
        }
        
        void extrude_height(float in_amount) {
            b -= in_amount;
            t += in_amount;
        }
        
        void scale_about_center(float in_scalar) {
            scale_width_about_center(in_scalar);
            scale_height_about_center(in_scalar);
        }
        
        void extrude(float in_amount) {
            extrude_width(in_amount);
            extrude_height(in_amount);
        }
        
        bounds4f flipx() {
            return bounds4f(t, l, b, r);
        }
        
        bounds4f flipy() {
            return bounds4f(b, r, t, l);
        }
        
        void anchor_in_parent(const bounds4f & in_parent, anchoring in_anchoring) {
            bounds4f l_result = in_parent.sub_bounds_with_size(size(), in_anchoring);
            t = l_result.t;
            r = l_result.r;
            b = l_result.b;
            l = l_result.l;
        }
        
        bounds4f sub_bounds_with_size(const size2f & in_size, anchoring in_anchoring) const
        {
            bounds4f l_result;
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
            if(in_p.x < l ||
               in_p.y < b ||
               in_p.x > r ||
               in_p.y > t)
                return false;
            return true;
        }
        
        bool touches(const bounds4f & in_otherBounds) const
        {
            if(l > in_otherBounds.r ||
               r < in_otherBounds.l ||
               b > in_otherBounds.t ||
               t < in_otherBounds.b)
                return false;
            return true;
        }
        
        bool contains(const bounds4f & in_otherBounds) const
        {
            if(l > in_otherBounds.l &&
               r < in_otherBounds.r &&
               b > in_otherBounds.b &&
               t < in_otherBounds.t)
                return true;
            return false;
        }
        
        void interpolate(const bounds4f & in_to, float in_val)
        {
            t = interpf(t, in_to.t, in_val);
            r = interpf(r, in_to.r, in_val);
            b = interpf(b, in_to.b, in_val);
            l = interpf(l, in_to.l, in_val);
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
        
        void extrude(int32_t in_amount)
        {
            t += in_amount;
            r += in_amount;
            b -= in_amount;
            l -= in_amount;
        }
        
        bool overlaps(const bounds4i & in_otherBounds) const {
            if(l > in_otherBounds.r ||
               r < in_otherBounds.l ||
               b > in_otherBounds.t ||
               t < in_otherBounds.b)
                return false;
            return true;
        };
    };
}