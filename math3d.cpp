

#include "math3d.h"

const atl::point3f atl::point3f::AxisX(1.f, 0.f, 0.f);
const atl::point3f atl::point3f::AxisY(0.f, 1.f, 0.f);
const atl::point3f atl::point3f::AxisZ(0.f, 0.f, 1.f);
const atl::point3f atl::point3f::Zero(0.f, 0.f, 0.f);

const atl::matrix4f atl::matrix4f::Identity(1.f, 0.f, 0.f, 0.f,
                                            0.f, 1.f, 0.f, 0.f,
                                            0.f, 0.f, 1.f, 0.f,
                                            0.f, 0.f, 0.f, 1.f);

atl::matrix4f atl::matrix4f::getInverse() const
{
    float a[16] = {
        m[0][0], m[0][1], m[0][2], m[0][3],
        m[1][0], m[1][1], m[1][2], m[1][3],
        m[2][0], m[2][1], m[2][2], m[2][3],
        m[3][0], m[3][1], m[3][2], m[3][3]
    };
    float inv[16], det;
    
    inv[0] = a[5]*(a[10]*a[15] - a[11]*a[14]) - a[9]*(a[6]*a[15] -
                                                      a[7]*a[14]) - a[13]*(a[7]*a[10] - a[6]*a[11]);
    inv[1] = a[1]*(a[11]*a[14] - a[10]*a[15]) - a[9]*(a[3]*a[14] -
                                                      a[2]*a[15]) - a[13]*(a[2]*a[11] - a[3]*a[10]);
    inv[2] = a[1]*(a[ 6]*a[15] - a[ 7]*a[14]) - a[5]*(a[2]*a[15] -
                                                      a[3]*a[14]) - a[13]*(a[3]*a[ 6] - a[2]*a[ 7]);
    inv[3] = a[1]*(a[ 7]*a[10] - a[ 6]*a[11]) - a[5]*(a[3]*a[10] -
                                                      a[2]*a[11]) - a[ 9]*(a[2]*a[ 7] - a[3]*a[ 6]);
    
    inv[4] = a[4]*(a[11]*a[14] - a[10]*a[15]) - a[8]*(a[7]*a[14] -
                                                      a[6]*a[15]) - a[12]*(a[6]*a[11] - a[7]*a[10]);
    inv[5] = a[0]*(a[10]*a[15] - a[11]*a[14]) - a[8]*(a[2]*a[15] -
                                                      a[3]*a[14]) - a[12]*(a[3]*a[10] - a[2]*a[11]);
    inv[6] = a[0]*(a[ 7]*a[14] - a[ 6]*a[15]) - a[4]*(a[3]*a[14] -
                                                      a[2]*a[15]) - a[12]*(a[2]*a[ 7] - a[3]*a[ 6]);
    inv[7] = a[0]*(a[ 6]*a[11] - a[ 7]*a[10]) - a[4]*(a[2]*a[11] -
                                                      a[3]*a[10]) - a[ 8]*(a[3]*a[ 6] - a[2]*a[ 7]);
    
    inv[8]  = a[4]*(a[ 9]*a[15] - a[11]*a[13]) - a[8]*(a[5]*a[15] -
                                                       a[7]*a[13]) - a[12]*(a[7]*a[ 9] - a[5]*a[11]);
    inv[9]  = a[0]*(a[11]*a[13] - a[ 9]*a[15]) - a[8]*(a[3]*a[13] -
                                                       a[1]*a[15]) - a[12]*(a[1]*a[11] - a[3]*a[ 9]);
    inv[10] = a[0]*(a[ 5]*a[15] - a[ 7]*a[13]) - a[4]*(a[1]*a[15] -
                                                       a[3]*a[13]) - a[12]*(a[3]*a[ 5] - a[1]*a[ 7]);
    inv[11] = a[0]*(a[ 7]*a[ 9] - a[ 5]*a[11]) - a[4]*(a[3]*a[ 9] -
                                                       a[1]*a[11]) - a[ 8]*(a[1]*a[ 7] - a[3]*a[ 5]); 
    
    inv[12] = a[4]*(a[10]*a[13] - a[ 9]*a[14]) - a[8]*(a[6]*a[13] - 
                                                       a[5]*a[14]) - a[12]*(a[5]*a[10] - a[6]*a[ 9]); 
    inv[13] = a[0]*(a[ 9]*a[14] - a[10]*a[13]) - a[8]*(a[1]*a[14] - 
                                                       a[2]*a[13]) - a[12]*(a[2]*a[ 9] - a[1]*a[10]); 
    inv[14] = a[0]*(a[ 6]*a[13] - a[ 5]*a[14]) - a[4]*(a[2]*a[13] - 
                                                       a[1]*a[14]) - a[12]*(a[1]*a[ 6] - a[2]*a[ 5]); 
    inv[15] = a[0]*(a[ 5]*a[10] - a[ 6]*a[ 9]) - a[4]*(a[1]*a[10] - 
                                                       a[2]*a[ 9]) - a[ 8]*(a[2]*a[ 5] - a[1]*a[ 6]);
    
    det = a[0] * inv[0] + a[1] * inv[4] + a[2] * inv[8] + a[3] * inv[12];
    
    det = 1.0f / det;
    
    return {
        inv[0] * det, inv[1] * det, inv[2] * det, inv[3] * det,
        inv[4] * det, inv[5] * det, inv[6] * det, inv[7] * det,
        inv[8] * det, inv[9] * det, inv[10] * det, inv[11] * det,
        inv[12] * det, inv[13] * det, inv[14] * det, inv[15] * det
    };
}

atl::matrix4f atl::matrix4f::getInverseAffine() const
{
    float a11 = m[0][0];
    float a12 = m[0][1];
    float a13 = m[0][2];
    float a21 = m[1][0];
    float a22 = m[1][1];
    float a23 = m[1][2];
    float a31 = m[2][0];
    float a32 = m[2][1];
    float a33 = m[2][2];
    float aX = m[3][0];
    float aY = m[3][1];
    float aZ = m[3][2];
    float aW = m[3][3];
    
    float inv[9] = {
        a22 * a33 - a23 * a32,
        a13 * a32 - a12 * a33,
        a12 * a23 - a13 * a22,
        
        a23 * a31 - a21 * a33,
        a11 * a33 - a13 * a31,
        a13 * a21 - a11 * a23,
        
        a21 * a32 - a22 * a31,
        a12 * a31 - a11 * a32,
        a11 * a22 - a12 * a21,
    };
    
    float det = a11 * inv[0] + a21 * inv[1] + a31 * inv[2];
    
    det = 1.0f / det;
    
    inv[0] *= det;
    inv[1] *= det;
    inv[2] *= det;
    inv[3] *= det;
    inv[4] *= det;
    inv[5] *= det;
    inv[6] *= det;
    inv[7] *= det;
    inv[8] *= det;
    
    float invPosX = aX * inv[0] + aY * inv[3] + aZ * inv[6] * aW;
    float invPosY = aX * inv[1] + aY * inv[4] + aZ * inv[7] * aW;
    float invPosZ = aX * inv[2] + aY * inv[5] + aZ * inv[8] * aW;
    
    return {
        inv[0], inv[1], inv[2], 0.f,
        inv[3], inv[4], inv[5], 0.f,
        inv[6], inv[7], inv[8], 0.f,
        -invPosX, -invPosY, -invPosZ, aW
    };
}
