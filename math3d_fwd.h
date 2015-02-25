

#pragma once

#include "basic_math.h"
#include <cmath>
#include <algorithm>

namespace atl
{
    class point3f
    {
    public:
        class vecLength
        {
        public:
            vecLength(float in_vX, float in_vY, float in_vZ) :
            _squaredValue(in_vX * in_vX + in_vY * in_vY + in_vZ * in_vZ) {}
            
            bool operator <(const vecLength & in_otherLength) const {
                return _squaredValue < in_otherLength._squaredValue;
            }
            
            bool operator >(const vecLength & in_otherLength) const {
                return _squaredValue > in_otherLength._squaredValue;
            }
            
            float getValue() const {
                return sqrtf(_squaredValue);
            }
            
            static vecLength forRadius(float in_radius) { return vecLength(in_radius, 0.f, 0.f); }
            
        private:
            float _squaredValue;
        };
        
        const static point3f X;
        const static point3f Y;
        const static point3f Z;
        const static point3f Zero;
        
        float x, y, z;
        
        point3f(float in_x, float in_y, float in_z) : x(in_x), y(in_y), z(in_z) {};
        point3f() {};
        
        void set(float in_x, float in_y, float in_z) {
            x = in_x;
            y = in_y;
            z = in_z;
        }
        
        bool operator == (const point3f & in_otherPoint) const {
            return x == in_otherPoint.x && y == in_otherPoint.y && z == in_otherPoint.z;
        }
        
        bool operator != (const point3f & in_otherPoint) const {
            return x != in_otherPoint.x || y != in_otherPoint.y || z != in_otherPoint.z;
        }
        
        void interpolate(const point3f & in_to, float in_val)
        {
            x = interpf(x, in_to.x, in_val);
            y = interpf(y, in_to.y, in_val);
            z = interpf(z, in_to.z, in_val);
        }

        point3f operator - () const {
            return point3f(-x, -y, -z);
        }
        
        vecLength length() const {
            return vecLength(x, y, z);
        }
        
        void normalize() {
            float currentLength = length().getValue();
            x = x / currentLength;
            y = y / currentLength;
            z = z / currentLength;
        }
        
        float dot() const {
            return x * x + y * y + z * z;
        }

        float dot(const point3f & in_other) const {
            return x * in_other.x + y * in_other.y + z * in_other.z;
        }
        
        point3f getNormal() const {
            float currentLength = length().getValue();
            return point3f(x / currentLength, y / currentLength, z / currentLength);
        };
        
        point3f & operator += (const point3f & in_vec) {
            x += in_vec.x;
            y += in_vec.y;
            z += in_vec.z;
            return *this;
        }
        
        point3f & operator -= (const point3f & in_vec) {
            x -= in_vec.x;
            y -= in_vec.y;
            z -= in_vec.z;
            return *this;
        }
        
        point3f & operator *= (float in_scalar) {
            x *= in_scalar;
            y *= in_scalar;
            z *= in_scalar;
            return *this;
        }
        
        point3f & operator /= (float in_scalar) {
            x /= in_scalar;
            y /= in_scalar;
            z /= in_scalar;
            return *this;
        }
        
        point3f operator * (float in_scalar) const {
            return point3f(x * in_scalar, y * in_scalar, z * in_scalar);
        }
        
        point3f operator / (float in_scalar) const {
            return point3f(x / in_scalar, y / in_scalar, z / in_scalar);
        }
        
        point3f operator + (const point3f & in_otherPoint) const
        {
            return point3f(x + in_otherPoint.x, y + in_otherPoint.y, z + in_otherPoint.z);
        }
        
        point3f operator - (const point3f & in_otherPoint) const
        {
            return point3f(x - in_otherPoint.x, y - in_otherPoint.y, z + in_otherPoint.z);
        }
        
        /*
        point3f getCross(const point3f & in_axis) const {
            return point3f(-y, x);
        };
        
        static point2f ForAngle(float in_angle, const point3f & in_axis)
        {
            return point2f(cosf(in_angle), sinf(in_angle));
        }
         */
    };
}