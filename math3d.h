

#pragma once

#include "basic_math.h"
#include <cmath>
#include <algorithm>
#include <array>

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
    
    class matrix4f
    {
    public:
        float m[4][4];
        
        matrix4f(float m00, float m01, float m02, float m03,
                 float m10, float m11, float m12, float m13,
                 float m20, float m21, float m22, float m23,
                 float m30, float m31, float m32, float m33) :
        m { m00, m01, m02, m03,
            m10, m11, m12, m13,
            m20, m21, m22, m23,
            m30, m31, m32, m33 }
        {}
        matrix4f() {}
        
        const static matrix4f Identity;
        
        class Col
        {
        public:
            float & r0;
            float & r1;
            float & r2;
            float & r3;
            
            Col(float & inR0, float & inR1, float & inR2, float & inR3) :
            r0(inR0),
            r1(inR1),
            r2(inR2),
            r3(inR3)
            {}
        };
        
        class Row
        {
        public:
            float & c0;
            float & c1;
            float & c2;
            float & c3;
            
            Row(float & inC0, float & inC1, float & inC2, float & inC3) :
            c0(inC0),
            c1(inC1),
            c2(inC2),
            c3(inC3)
            {}
        };
        
        Row row(int32_t idx)
        {
            return {m[idx][0], m[idx][1], m[idx][2], m[idx][3]};
        }

        Col col(int32_t idx)
        {
            return {m[0][idx], m[1][idx], m[2][idx], m[3][idx]};
        }
        
        std::array<float, 4> row(int32_t idx) const
        {
            return {m[idx][0], m[idx][1], m[idx][2], m[idx][3]};
        }

        std::array<float, 4> col(int32_t idx) const
        {
            return {m[0][idx], m[1][idx], m[2][idx], m[3][idx]};
        }
        
        void setPosition(const atl::point3f & inPos)
        {
            m[3][0] = inPos.x;
            m[3][1] = inPos.y;
            m[3][2] = inPos.z;
        }
        
        bool operator == (const matrix4f & otherMatrix) const
        {
            return (m[0][0] == otherMatrix.m[0][0] &&
                    m[0][1] == otherMatrix.m[0][1] &&
                    m[0][2] == otherMatrix.m[0][2] &&
                    m[0][3] == otherMatrix.m[0][3] &&
                    m[1][0] == otherMatrix.m[1][0] &&
                    m[1][1] == otherMatrix.m[1][1] &&
                    m[1][2] == otherMatrix.m[1][2] &&
                    m[1][3] == otherMatrix.m[1][3] &&
                    m[2][0] == otherMatrix.m[2][0] &&
                    m[2][1] == otherMatrix.m[2][1] &&
                    m[2][2] == otherMatrix.m[2][2] &&
                    m[2][3] == otherMatrix.m[2][3] &&
                    m[3][0] == otherMatrix.m[3][0] &&
                    m[3][1] == otherMatrix.m[3][1] &&
                    m[3][2] == otherMatrix.m[3][2] &&
                    m[3][3] == otherMatrix.m[3][3]);
        }
        
        bool operator != (const matrix4f & otherMatrix) const
        {
            return (m[0][0] != otherMatrix.m[0][0] ||
                    m[0][1] != otherMatrix.m[0][1] ||
                    m[0][2] != otherMatrix.m[0][2] ||
                    m[0][3] != otherMatrix.m[0][3] ||
                    m[1][0] != otherMatrix.m[1][0] ||
                    m[1][1] != otherMatrix.m[1][1] ||
                    m[1][2] != otherMatrix.m[1][2] ||
                    m[1][3] != otherMatrix.m[1][3] ||
                    m[2][0] != otherMatrix.m[2][0] ||
                    m[2][1] != otherMatrix.m[2][1] ||
                    m[2][2] != otherMatrix.m[2][2] ||
                    m[2][3] != otherMatrix.m[2][3] ||
                    m[3][0] != otherMatrix.m[3][0] ||
                    m[3][1] != otherMatrix.m[3][1] ||
                    m[3][2] != otherMatrix.m[3][2] ||
                    m[3][3] != otherMatrix.m[3][3]);
        }
        
        matrix4f transform(const matrix4f & b) const
        {
            return {
                m[0][0] * b.m[0][0] + m[1][0] * b.m[0][1] + m[2][0] * b.m[0][2] + m[3][0] * b.m[0][3],
                m[0][1] * b.m[0][1] + m[1][1] * b.m[0][1] + m[2][1] * b.m[0][2] + m[3][1] * b.m[0][3],
                m[0][2] * b.m[0][2] + m[1][2] * b.m[0][1] + m[2][2] * b.m[0][2] + m[3][2] * b.m[0][3],
                m[0][3] * b.m[0][3] + m[1][3] * b.m[0][1] + m[2][3] * b.m[0][2] + m[3][3] * b.m[0][3],
                
                m[0][0] * b.m[1][0] + m[1][0] * b.m[1][1] + m[2][0] * b.m[1][2] + m[3][0] * b.m[1][3],
                m[0][1] * b.m[1][1] + m[1][1] * b.m[1][1] + m[2][1] * b.m[1][2] + m[3][1] * b.m[1][3],
                m[0][2] * b.m[1][2] + m[1][2] * b.m[1][1] + m[2][2] * b.m[1][2] + m[3][2] * b.m[1][3],
                m[0][3] * b.m[1][3] + m[1][3] * b.m[1][1] + m[2][3] * b.m[1][2] + m[3][3] * b.m[1][3],
                
                m[0][0] * b.m[2][0] + m[1][0] * b.m[2][1] + m[2][0] * b.m[2][2] + m[3][0] * b.m[2][3],
                m[0][1] * b.m[2][1] + m[1][1] * b.m[2][1] + m[2][1] * b.m[2][2] + m[3][1] * b.m[2][3],
                m[0][2] * b.m[2][2] + m[1][2] * b.m[2][1] + m[2][2] * b.m[2][2] + m[3][2] * b.m[2][3],
                m[0][3] * b.m[2][3] + m[1][3] * b.m[2][1] + m[2][3] * b.m[2][2] + m[3][3] * b.m[2][3],
                
                m[0][0] * b.m[3][0] + m[1][0] * b.m[3][1] + m[2][0] * b.m[3][2] + m[3][0] * b.m[3][3],
                m[0][1] * b.m[3][1] + m[1][1] * b.m[3][1] + m[2][1] * b.m[3][2] + m[3][1] * b.m[3][3],
                m[0][2] * b.m[3][2] + m[1][2] * b.m[3][1] + m[2][2] * b.m[3][2] + m[3][2] * b.m[3][3],
                m[0][3] * b.m[3][3] + m[1][3] * b.m[3][1] + m[2][3] * b.m[3][2] + m[3][3] * b.m[3][3],
            };
        }
        
        point3f transform(const point3f & inPoint) const
        {
            return {
                inPoint.x * m[0][0] + inPoint.y * m[1][0] + inPoint.z * m[2][0] + m[3][0],
                inPoint.x * m[0][1] + inPoint.y * m[1][1] + inPoint.z * m[2][1] + m[3][1],
                inPoint.x * m[0][2] + inPoint.y * m[1][2] + inPoint.z * m[2][2] + m[3][2],
            };
        }
        
        void getEigenvalues() const
        {
            // TODO:
        }
        
        void getDeterminant() const
        {
            // TODO:
        }
        
        void getInverse() const
        {
            // TODO:
        }
        
        void inverse()
        {
            // TODO:
            
        }
        
        matrix4f getTranspose() const
        {
            return {
                m[0][0], m[1][0], m[2][0], m[3][0],
                m[0][1], m[1][1], m[2][1], m[3][1],
                m[0][2], m[1][2], m[2][2], m[3][2],
                m[0][3], m[1][3], m[2][3], m[3][3],
            };
        }
        
        matrix4f & transpose()
        {
            std::swap(m[0][1], m[1][0]);
            std::swap(m[0][2], m[2][0]);
            std::swap(m[0][3], m[3][0]);
            std::swap(m[1][2], m[2][1]);
            std::swap(m[2][3], m[3][2]);
            std::swap(m[1][3], m[3][1]);
            return *this;
        }
        
        matrix4f & operator += (const matrix4f & otherMatrix)
        {
            m[0][0] += otherMatrix.m[0][0];
            m[0][1] += otherMatrix.m[0][1];
            m[0][2] += otherMatrix.m[0][2];
            m[0][3] += otherMatrix.m[0][3];
            m[1][0] += otherMatrix.m[1][0];
            m[1][1] += otherMatrix.m[1][1];
            m[1][2] += otherMatrix.m[1][2];
            m[1][3] += otherMatrix.m[1][3];
            m[2][0] += otherMatrix.m[2][0];
            m[2][1] += otherMatrix.m[2][1];
            m[2][2] += otherMatrix.m[2][2];
            m[2][3] += otherMatrix.m[2][3];
            m[3][0] += otherMatrix.m[3][0];
            m[3][1] += otherMatrix.m[3][1];
            m[3][2] += otherMatrix.m[3][2];
            m[3][3] += otherMatrix.m[3][3];
            return *this;
        }
    
        matrix4f & operator -= (const matrix4f & otherMatrix)
        {
            m[0][0] -= otherMatrix.m[0][0];
            m[0][1] -= otherMatrix.m[0][1];
            m[0][2] -= otherMatrix.m[0][2];
            m[0][3] -= otherMatrix.m[0][3];
            m[1][0] -= otherMatrix.m[1][0];
            m[1][1] -= otherMatrix.m[1][1];
            m[1][2] -= otherMatrix.m[1][2];
            m[1][3] -= otherMatrix.m[1][3];
            m[2][0] -= otherMatrix.m[2][0];
            m[2][1] -= otherMatrix.m[2][1];
            m[2][2] -= otherMatrix.m[2][2];
            m[2][3] -= otherMatrix.m[2][3];
            m[3][0] -= otherMatrix.m[3][0];
            m[3][1] -= otherMatrix.m[3][1];
            m[3][2] -= otherMatrix.m[3][2];
            m[3][3] -= otherMatrix.m[3][3];
            return *this;
        }
        
        matrix4f & operator *= (float scalar)
        {
            m[0][0] *= scalar;
            m[0][1] *= scalar;
            m[0][2] *= scalar;
            m[0][3] *= scalar;
            m[1][0] *= scalar;
            m[1][1] *= scalar;
            m[1][2] *= scalar;
            m[1][3] *= scalar;
            m[2][0] *= scalar;
            m[2][1] *= scalar;
            m[2][2] *= scalar;
            m[2][3] *= scalar;
            m[3][0] *= scalar;
            m[3][1] *= scalar;
            m[3][2] *= scalar;
            m[3][3] *= scalar;
            return *this;
        }
        
        matrix4f & operator /= (float scalar)
        {
            m[0][0] /= scalar;
            m[0][1] /= scalar;
            m[0][2] /= scalar;
            m[0][3] /= scalar;
            m[1][0] /= scalar;
            m[1][1] /= scalar;
            m[1][2] /= scalar;
            m[1][3] /= scalar;
            m[2][0] /= scalar;
            m[2][1] /= scalar;
            m[2][2] /= scalar;
            m[2][3] /= scalar;
            m[3][0] /= scalar;
            m[3][1] /= scalar;
            m[3][2] /= scalar;
            m[3][3] /= scalar;
            return *this;
        }
        
        matrix4f operator * (float in_scalar) const
        {
            return {
                m[0][0] * in_scalar, m[0][1] * in_scalar, m[0][2] * in_scalar, m[0][3] * in_scalar,
                m[1][0] * in_scalar, m[1][1] * in_scalar, m[1][2] * in_scalar, m[1][3] * in_scalar,
                m[2][0] * in_scalar, m[2][1] * in_scalar, m[2][2] * in_scalar, m[2][3] * in_scalar,
                m[3][0] * in_scalar, m[3][1] * in_scalar, m[3][2] * in_scalar, m[3][3] * in_scalar};
        }
        
        matrix4f operator / (float in_scalar) const
        {
            return {
                m[0][0] / in_scalar, m[0][1] / in_scalar, m[0][2] / in_scalar, m[0][3] / in_scalar,
                m[1][0] / in_scalar, m[1][1] / in_scalar, m[1][2] / in_scalar, m[1][3] / in_scalar,
                m[2][0] / in_scalar, m[2][1] / in_scalar, m[2][2] / in_scalar, m[2][3] / in_scalar,
                m[3][0] / in_scalar, m[3][1] / in_scalar, m[3][2] / in_scalar, m[3][3] / in_scalar};
        }
        
        matrix4f operator + (const matrix4f & otherMatrix) const
        {
            return {
                m[0][0] + otherMatrix.m[0][0], m[0][1] + otherMatrix.m[0][1], m[0][2] + otherMatrix.m[0][2], m[0][3] + otherMatrix.m[0][3],
                m[1][0] + otherMatrix.m[1][0], m[1][1] + otherMatrix.m[1][1], m[1][2] + otherMatrix.m[1][2], m[1][3] + otherMatrix.m[1][3],
                m[2][0] + otherMatrix.m[2][0], m[2][1] + otherMatrix.m[2][1], m[2][2] + otherMatrix.m[2][2], m[2][3] + otherMatrix.m[2][3],
                m[3][0] + otherMatrix.m[3][0], m[3][1] + otherMatrix.m[3][1], m[3][2] + otherMatrix.m[3][2], m[3][3] + otherMatrix.m[3][3]};
        }
        
        matrix4f operator - (const matrix4f & otherMatrix) const
        {
            return {
                m[0][0] - otherMatrix.m[0][0], m[0][1] - otherMatrix.m[0][1], m[0][2] - otherMatrix.m[0][2], m[0][3] - otherMatrix.m[0][3],
                m[1][0] - otherMatrix.m[1][0], m[1][1] - otherMatrix.m[1][1], m[1][2] - otherMatrix.m[1][2], m[1][3] - otherMatrix.m[1][3],
                m[2][0] - otherMatrix.m[2][0], m[2][1] - otherMatrix.m[2][1], m[2][2] - otherMatrix.m[2][2], m[2][3] - otherMatrix.m[2][3],
                m[3][0] - otherMatrix.m[3][0], m[3][1] - otherMatrix.m[3][1], m[3][2] - otherMatrix.m[3][2], m[3][3] - otherMatrix.m[3][3]};
        }
        
        static matrix4f FrustumProjection()
        {
            // TODO:
            return Identity;
        }
        
        static matrix4f LookAt()
        {
            // TODO:
            return Identity;
        }
        
        static matrix4f OrthographicProjection()
        {
            // TODO:
            return Identity;
        }
        
        static matrix4f Translation(const point3f & inTranslation)
        {
            return {
                1.f, 0.f, 0.f, 0.f,
                0.f, 1.f, 0.f, 0.f,
                0.f, 0.f, 1.f, 0.f,
                inTranslation.x, inTranslation.y, inTranslation.z, 1.f
            };
        }
        
        static matrix4f RotationX(float inAngle)
        {
            float lC = cosf(inAngle);
            float lS = sinf(inAngle);
            return {
                1.f, 0.f, 0.f, 0.f,
                0.f,  lC, -lS, 0.f,
                0.f,  lS,  lC, 0.f,
                0.f, 0.f, 0.f, 1.f
            };
        }

        static matrix4f RotationY(float inAngle)
        {
            float lC = cosf(inAngle);
            float lS = sinf(inAngle);
            return {
                 lC, 0.f,  lS, 0.f,
                0.f, 1.f, 0.f, 0.f,
                -lS, 0.f,  lC, 0.f,
                0.f, 0.f, 0.f, 1.f
            };
        }

        static matrix4f RotationZ(float inAngle)
        {
            float lC = cosf(inAngle);
            float lS = sinf(inAngle);
            return {
                 lC, -lS, 0.f, 0.f,
                 lS,  lC, 0.f, 0.f,
                0.f, 0.f, 1.f, 0.f,
                0.f, 0.f, 0.f, 1.f
            };
        }
        
        static matrix4f AxisAngleRotation(const point3f & inAxis, float inAngle)
        {
            float lC = cosf(inAngle);
            float lS = sinf(inAngle);
            float lCM = 1.f - lC;
            return {
                inAxis.x * inAxis.x * lCM + lC, inAxis.x * inAxis.y * lCM - inAxis.z * lS, inAxis.x * inAxis.z * lCM + inAxis.y * lS, 0.f,
                inAxis.y * inAxis.x * lCM + inAxis.z * lS, inAxis.y * inAxis.y * lCM + lC, inAxis.y * inAxis.y * lCM - inAxis.x * lS, 0.f,
                inAxis.z * inAxis.x * lCM - inAxis.y * lS, inAxis.z * inAxis.y * lCM + inAxis.x * lS, inAxis.z * inAxis.z * lCM + lC, 0.f,
                0.f, 0.f, 0.f, 1.f
            };
        }
        
        static matrix4f Scale(const point3f & inScale)
        {
            return {
                inScale.x, 0.f, 0.f, 0.f,
                0.f, inScale.y, 0.f, 0.f,
                0.f, 0.f, inScale.z, 0.f,
                0.f, 0.f, 0.f, 1.f
            };
        }
    };
}