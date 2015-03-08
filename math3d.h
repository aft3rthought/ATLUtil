

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
        class vector_length
        {
        public:
            vector_length(float in_x, float in_y, float in_z) :
            squared_value(in_x * in_x + in_y * in_y + in_z * in_z) {}
            
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
                return {in_radius, 0.f, 0.f};
            }
            
            float squared_value;
        };
        
        const static point3f AxisX;
        const static point3f AxisY;
        const static point3f AxisZ;
        const static point3f Zero;
        
        float x, y, z;
        
        point3f(float in_x, float in_y, float in_z) :
        x(in_x),
        y(in_y),
        z(in_z)
        {};
        
        point3f() {};
        
        point3f & set(float in_x, float in_y, float in_z) {
            x = in_x;
            y = in_y;
            z = in_z;
            return *this;
        }
        
        bool operator == (const point3f & in_otherPoint) const {
            return x == in_otherPoint.x && y == in_otherPoint.y && z == in_otherPoint.z;
        }
        
        bool operator != (const point3f & in_otherPoint) const {
            return x != in_otherPoint.x || y != in_otherPoint.y || z != in_otherPoint.z;
        }
        
        point3f & interpolate(const point3f & in_to, float in_val)
        {
            x = interpf(x, in_to.x, in_val);
            y = interpf(y, in_to.y, in_val);
            z = interpf(z, in_to.z, in_val);
            return *this;
        }

        point3f operator - () const {
            return {-x, -y, -z};
        }
        
        vector_length length() const {
            return vector_length(x, y, z);
        }
        
        point3f & normalize() {
            return *this /= length().get_value();
        }
        
        point3f get_normal() const {
            return *this / length().get_value();
        };
        
        float dot(const point3f & in_otherPoint) const {
            return x * in_otherPoint.x + y * in_otherPoint.y + z * in_otherPoint.z;
        }
        
        float dot() const {
            return dot(*this);
        }
        
        point3f & operator += (const point3f & in_otherPoint) {
            x += in_otherPoint.x;
            y += in_otherPoint.y;
            z += in_otherPoint.z;
            return *this;
        }
        
        point3f & operator -= (const point3f & in_otherPoint) {
            x -= in_otherPoint.x;
            y -= in_otherPoint.y;
            z -= in_otherPoint.z;
            return *this;
        }
        
        point3f & operator *= (float in_scalar) {
            x *= in_scalar;
            y *= in_scalar;
            z *= in_scalar;
            return *this;
        }
        
        point3f & operator /= (float in_scalar) {
            in_scalar = 1.f / in_scalar;
            x *= in_scalar;
            y *= in_scalar;
            z *= in_scalar;
            return *this;
        }
        
        point3f operator * (float in_scalar) const {
            return point3f(*this) *= in_scalar;
        }
        
        point3f operator / (float in_scalar) const {
            return point3f(*this) /= in_scalar;
        }
        
        point3f operator + (const point3f & in_otherPoint) const {
            return point3f(*this) += in_otherPoint;
        }
        
        point3f operator - (const point3f & in_otherPoint) const {
            return point3f(*this) -= in_otherPoint;
        }

        point3f get_projection(const point3f & in_otherPoint) const {
            return point3f(*this) * dot(in_otherPoint) / dot();
        }
        
        point3f get_cross(const point3f & in_otherPoint) const {
            return {
                y * in_otherPoint.z - z * in_otherPoint.y,
                z * in_otherPoint.x - x * in_otherPoint.z,
                x * in_otherPoint.y - y * in_otherPoint.x
            };
        }
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
            return {m[0][idx], m[1][idx], m[2][idx], m[3][idx]};
        }

        Col col(int32_t idx)
        {
            return {m[idx][0], m[idx][1], m[idx][2], m[idx][3]};
        }
        
        std::array<float, 4> row(int32_t idx) const
        {
            return {m[0][idx], m[1][idx], m[2][idx], m[3][idx]};
        }

        std::array<float, 4> col(int32_t idx) const
        {
            return {m[idx][0], m[idx][1], m[idx][2], m[idx][3]};
        }
        
        void setPosition(const atl::point3f & inPos)
        {
            m[0][3] = inPos.x;
            m[1][3] = inPos.y;
            m[2][3] = inPos.z;
        }
        
        atl::point3f getPosition() const
        {
            return atl::point3f(m[0][3], m[1][3], m[2][3]);
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
                m[0][1] * b.m[0][0] + m[1][1] * b.m[0][1] + m[2][1] * b.m[0][2] + m[3][1] * b.m[0][3],
                m[0][2] * b.m[0][0] + m[1][2] * b.m[0][1] + m[2][2] * b.m[0][2] + m[3][2] * b.m[0][3],
                m[0][3] * b.m[0][0] + m[1][3] * b.m[0][1] + m[2][3] * b.m[0][2] + m[3][3] * b.m[0][3],
                
                m[0][0] * b.m[1][0] + m[1][0] * b.m[1][1] + m[2][0] * b.m[1][2] + m[3][0] * b.m[1][3],
                m[0][1] * b.m[1][0] + m[1][1] * b.m[1][1] + m[2][1] * b.m[1][2] + m[3][1] * b.m[1][3],
                m[0][2] * b.m[1][0] + m[1][2] * b.m[1][1] + m[2][2] * b.m[1][2] + m[3][2] * b.m[1][3],
                m[0][3] * b.m[1][0] + m[1][3] * b.m[1][1] + m[2][3] * b.m[1][2] + m[3][3] * b.m[1][3],
                
                m[0][0] * b.m[2][0] + m[1][0] * b.m[2][1] + m[2][0] * b.m[2][2] + m[3][0] * b.m[2][3],
                m[0][1] * b.m[2][0] + m[1][1] * b.m[2][1] + m[2][1] * b.m[2][2] + m[3][1] * b.m[2][3],
                m[0][2] * b.m[2][0] + m[1][2] * b.m[2][1] + m[2][2] * b.m[2][2] + m[3][2] * b.m[2][3],
                m[0][3] * b.m[2][0] + m[1][3] * b.m[2][1] + m[2][3] * b.m[2][2] + m[3][3] * b.m[2][3],
                
                m[0][0] * b.m[3][0] + m[1][0] * b.m[3][1] + m[2][0] * b.m[3][2] + m[3][0] * b.m[3][3],
                m[0][1] * b.m[3][0] + m[1][1] * b.m[3][1] + m[2][1] * b.m[3][2] + m[3][1] * b.m[3][3],
                m[0][2] * b.m[3][0] + m[1][2] * b.m[3][1] + m[2][2] * b.m[3][2] + m[3][2] * b.m[3][3],
                m[0][3] * b.m[3][0] + m[1][3] * b.m[3][1] + m[2][3] * b.m[3][2] + m[3][3] * b.m[3][3],
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
            // TODO: Can use QR Decomposition
        }
        
        void getDeterminant() const
        {
            // TODO:
        }
        
        matrix4f getInverse() const;
        matrix4f getInverseAffine() const;
        
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
                0.f,  lC,  lS, 0.f,
                0.f, -lS,  lC, 0.f,
                0.f, 0.f, 0.f, 1.f
            };
        }

        static matrix4f RotationY(float inAngle)
        {
            float lC = cosf(inAngle);
            float lS = sinf(inAngle);
            return {
                 lC, 0.f, -lS, 0.f,
                0.f, 1.f, 0.f, 0.f,
                 lS, 0.f,  lC, 0.f,
                0.f, 0.f, 0.f, 1.f
            };
        }

        static matrix4f RotationZ(float inAngle)
        {
            float lC = cosf(inAngle);
            float lS = sinf(inAngle);
            return {
                 lC,  lS, 0.f, 0.f,
                -lS,  lC, 0.f, 0.f,
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
                inAxis.x * inAxis.x * lCM + lC,             inAxis.y * inAxis.x * lCM + inAxis.z * lS,  inAxis.z * inAxis.x * lCM - inAxis.y * lS,  0.f,
                inAxis.x * inAxis.y * lCM - inAxis.z * lS,  inAxis.y * inAxis.y * lCM + lC,             inAxis.z * inAxis.y * lCM + inAxis.x * lS,  0.f,
                inAxis.x * inAxis.z * lCM + inAxis.y * lS,  inAxis.y * inAxis.z * lCM - inAxis.x * lS,  inAxis.z * inAxis.z * lCM + lC,             0.f,
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
        
        static matrix4f OpenGLFrustumProjection(float top,
                                                float right,
                                                float bottom,
                                                float left,
                                                float near,
                                                float far)
        {
            float w = right - left;
            float h = top - bottom;
            float d = far - near;
            float n2 = near * 2.f;
            float tx = right + left;
            float ty = top + bottom;
            float tz = far + near;
            float s = -2.f * far * near;
            return {
                n2 / w, 0.f, 0.f, 0.f,
                0.f, n2 / h, 0.f, 0.f,
                tx / w, ty / h, -(tz / d), -1.f,
                0.f, 0.f, s / d, 0.f };
        }
        
        static matrix4f OpenGLPerspectiveProjection(float verticalFov, float width, float height, float near, float far)
        {
            float yScale = 1.f / tanf(verticalFov / 2.f);
            float xScale = yScale / (width / height);
            float nmf = near - far;
            return {
                xScale, 0.f, 0.f, 0.f,
                0.f, yScale, 0.f, 0.f,
                0.f, 0.f, (near + far) / nmf, -1.f,
                0.f, 0.f, (2.f * far * near) / nmf, 0.f };
        }
        
        static matrix4f OpenGLLookAt()
        {
            // TODO:
            return Identity;
        }
        
        static matrix4f OpenGLOrthographicProjection(float top,
                                                     float right,
                                                     float bottom,
                                                     float left,
                                                     float near,
                                                     float far)
        {
            float w = right - left;
            float h = top - bottom;
            float d = far - near;
            float tx = right + left;
            float ty = top + bottom;
            float tz = far + near;
            return {
                2.f / w, 0.f, 0.f, 0.f,
                0.f, 2.f / h, 0.f, 0.f,
                0.f, 0.f, -2.f / d, 0.f,
                -tx / w, -ty / h, -tz / d, 1.f };
        }
        
        static matrix4f OpenGLOrthographicUnprojection(float top,
                                                       float right,
                                                       float bottom,
                                                       float left,
                                                       float near,
                                                       float far)
        {
            float w = right - left;
            float h = top - bottom;
            float d = far - near;
            float tx = right + left;
            float ty = top + bottom;
            float tz = far + near;
            return {
                w / 2.f, 0.f, 0.f, 0.f,
                0.f, h / 2.f, 0.f, 0.f,
                0.f, 0.f, d / -2.f, 0.f,
                tx / 2.f, ty / 2.f, tz / -2.f, 1.f };
        }        
    };
    
    class quatf
    {
    public:
        float x, y, z, w;
        
        quatf(float x, float y, float z, float w) :
        x(x),
        y(y),
        z(z),
        w(w)
        {}

        quatf(const point3f & inAxis, float inAngle)
        {
            float lHalfAngle = inAngle * 0.5f;
            float lScale = sinf(lHalfAngle);
            x = inAxis.x * lScale;
            y = inAxis.y * lScale;
            z = inAxis.z * lScale;
            w = cosf(lHalfAngle);
        }
        
        /*
         * Operations on quats
         */
        quatf& operator += (const quatf & inOtherQuat)
        {
            x += inOtherQuat.x;
            y += inOtherQuat.y;
            z += inOtherQuat.z;
            w += inOtherQuat.w;
            return *this;
        }
        
        quatf& operator -= (const quatf & inOtherQuat)
        {
            x -= inOtherQuat.x;
            y -= inOtherQuat.y;
            z -= inOtherQuat.z;
            w -= inOtherQuat.w;
            return *this;
        }
        
        quatf operator + (const quatf & inOtherQuat) const
        {
            return {
                x + inOtherQuat.x,
                y + inOtherQuat.y,
                z + inOtherQuat.z,
                w + inOtherQuat.w
            };
        }

        quatf operator - (const quatf & inOtherQuat) const
        {
            return {
                x - inOtherQuat.x,
                y - inOtherQuat.y,
                z - inOtherQuat.z,
                w - inOtherQuat.w
            };
        }
        
        quatf operator * (const quatf & inOtherQuat) const
        {
            return {
                w * inOtherQuat.x +
                x * inOtherQuat.w +
                y * inOtherQuat.z -
                z * inOtherQuat.y,
                
                w * inOtherQuat.y +
                y * inOtherQuat.w +
                z * inOtherQuat.x -
                x * inOtherQuat.z,
                
                w * inOtherQuat.z +
                z * inOtherQuat.w +
                x * inOtherQuat.y -
                y * inOtherQuat.x,
                
                w * inOtherQuat.w -
                x * inOtherQuat.x -
                y * inOtherQuat.y -
                z * inOtherQuat.z
            };
        }
        
        /*
         * Operations on points
         */
        point3f operator * (const point3f & inPoint) const
        {
            quatf lPointQuat(inPoint.x, inPoint.y, inPoint.z, 0.f);
            lPointQuat = (*this * lPointQuat) * getReciprocal();
            return {lPointQuat.x, lPointQuat.y, lPointQuat.z };
        }
        
        /*
         * Scalar operations
         */
        quatf& operator *= (float inScalar)
        {
            x *= inScalar;
            y *= inScalar;
            z *= inScalar;
            w *= inScalar;
            return *this;
        }
        
        quatf operator * (float inScalar) const
        {
            return {
                x * inScalar,
                y * inScalar,
                z * inScalar,
                w * inScalar
            };
        }
        
        quatf& operator /= (float inScalar)
        {
            inScalar = 1.f / inScalar;
            x /= inScalar;
            y /= inScalar;
            z /= inScalar;
            w /= inScalar;
            return *this;
        }
        
        quatf operator / (float inScalar) const
        {
            inScalar = 1.f / inScalar;
            return {
                x * inScalar,
                y * inScalar,
                z * inScalar,
                w * inScalar
            };
        }
        
        quatf operator - () const
        {
            return {-x, -y, -z, -w};
        }
        
        /*
         * Length and other properties
         */
        class quatLength
        {
        public:
            quatLength(float inX, float inY, float inZ, float inW) :
            squaredValue(inX * inX + inY * inY + inZ * inZ + inW * inW) {}
            
            bool operator <(const quatLength & inOtherLength) const {
                return squaredValue < inOtherLength.squaredValue;
            }
            
            bool operator >(const quatLength & inOtherLength) const {
                return squaredValue > inOtherLength.squaredValue;
            }
            
            float getValue() const {
                return sqrtf(squaredValue);
            }
            
            float squaredValue;
        };

        quatLength length() const
        {
            return quatLength(x, y, z, w);
        }
        
        float dot(const quatf & in_other_quat) const
        {
            return (x * in_other_quat.x +
                    y * in_other_quat.y +
                    z * in_other_quat.z +
                    w * in_other_quat.w);
        }
        
        quatf getConjugate() const
        {
            return { -x, -y, -z, w };
        }
        
        quatf getReciprocal() const
        {
            return getConjugate() / length().squaredValue;
        }

        quatf getUnitQuaternion() const
        {
            return quatf(*this) / length().getValue();
        }
        
        /*
         * Unit vectors
         */
        point3f xAxis() const
        {
            return {
                1.f - 2.f * y * y - 2.f * z * z,
                2.f * x * y - 2.f * z * w,
                2.f * x * z + 2.f * y * w
            };
        }
        
        point3f yAxis() const
        {
            return {
                2.f * x * y + 2.f * z * w,
                1.f - 2.f * x * x - 2.f * z * z,
                2.f * y * z - 2.f * x * w
            };
        }
        
        point3f zAxis() const
        {
            return {
                2.f * x * z - 2.f * y * w,
                2.f * y * z + 2.f * x * w,
                1.f - 2.f * x * x - 2.f * y * y
            };
        }
        
        /*
         * Lerps
         */
        
        /*
         * slerp (spherical linear interpolation)
         * Note: Not only is slerp slower than nlerp, but combining multiple slerps is non-commutative.
         *       Ideal for simple applications where accuracy is desired.
         */
        quatf slerp(const quatf & in_dest_quat, float in_t) const
        {
            float lDot = dot(in_dest_quat);
            if (lDot > 0.9995f)
                return nlerp(in_dest_quat, in_t);
            
            lDot = atl::clamp(lDot, -1.f, 1.f);
            float lAngleSweep = acosf(lDot) * in_t;
            
            quatf v2 = in_dest_quat - *this * lDot;
            v2.getUnitQuaternion();
            
            return *this * cosf(lAngleSweep) + v2*sinf(lAngleSweep);
        }
        
        /*
         * nlerp (normalized linear interpolation)
         */
        quatf nlerp(const quatf & in_dest_quat, float in_t) const
        {
            return (*this + (in_dest_quat - *this) * in_t).getUnitQuaternion();
/*
            float lDot = dot(in_dest_quat);
            if(lDot == 0.f)
            {
                quatf lMid = quatf(yAxis(), M_PI * 0.5f);
                if(in_t < 0.5f)
                {
                    in_t = in_t * 2.f;
                    return (*this + (lMid - *this) * in_t).getUnitQuaternion();
                }
                else
                {
                    in_t = (in_t - 0.5f) * 2.f;
                    return (lMid + (in_dest_quat - lMid) * in_t).getUnitQuaternion();
                }
            }
            else if(lDot < 0.f)
            {
                return (*this + (-in_dest_quat - *this) * in_t).getUnitQuaternion();
            }
            else
            {
                return (*this + (in_dest_quat - *this) * in_t).getUnitQuaternion();
            }
*/
        }
    };
}