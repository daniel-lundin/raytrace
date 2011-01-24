#include "matrix.h"
#include <math.h>

#define PI 3.14159265

void generateXRot(MATRIX3D& m, float degree)
{
    float rad = degree*(PI/180);
    memset(m, 0, 3*3*sizeof(float));
    // Setup rotation matrix around x-axis
    m[0][0] = 1;
    m[1][1] = m[2][2] = cos(rad);
    m[1][2] = - sin(rad);
    m[2][1] = sin(rad);
}
void generateYRot(MATRIX3D& m, float degree)
{

    float rad = degree*(PI/180);
    memset(m, 0, 3*3*sizeof(float));
    // Setup rotation matrix around y-axis
    m[0][0] = m[2][2] = cos(rad);
    m[1][1] = 1;
    m[0][2] = sin(rad);
    m[2][0] = -sin(rad);
}
void generateZRot(MATRIX3D& m, float degree)
{
    float rad = degree*(PI/180);
    memset(m, 0, 3*3*sizeof(float));
    // Setup rotation matrix around z-axis
    m[0][0] = m[1][1] = cos(rad);
    m[2][2] = 1;
    m[0][1] = -sin(rad);
    m[1][0] = sin(rad);
}

void transpose(const MATRIX3D& in, MATRIX3D& out)
{
    for (int i = 0; i < 3; i++) 
        for (int j = 0; j < 3; j++) 
            out[i][j] = in[j][i];
}

Vector3D apply(const MATRIX3D& m, const Vector3D& v)
{
    float vec[3];
    for (int i = 0; i < 3; i++) 
    {
        vec[i] = m[i][0]*v.x() + m[i][1]*v.y() + m[i][2]*v.z();
    }
    return Vector3D(vec[0], vec[1], vec[2]);
}

void apply(const MATRIX3D& a, const MATRIX3D& b, MATRIX3D& res)
{
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            res[i][j] = a[i][0]*b[0][j] +
                        a[i][1]*b[1][j] +
                        a[i][2]*b[2][j];
        }
    }
}
