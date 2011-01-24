#ifndef MATRIX_HPP
#define MATRIX_HPP
#include "vector3d.h"
typedef float MATRIX3D[3][3];

// Some initializers
void generateXRot(MATRIX3D&, float degree);
void generateYRot(MATRIX3D&, float degree);
void generateZRot(MATRIX3D&, float degree);
// Some operations
void transpose(const MATRIX3D& in, MATRIX3D& out);
Vector3D apply(const MATRIX3D& m, const Vector3D& v);
void apply(const MATRIX3D& a, const MATRIX3D& b, MATRIX3D& res);
#endif
