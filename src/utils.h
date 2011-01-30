#ifndef UTILS_H
#define UTILS_H
#include "vector3d.h"
#include "intersection.h"

Vector3D randomDisplaceVector(const Vector3D& v, float max);
Vector3D roundVector(const Vector3D& v);
Vector3D sinify(const Vector3D& v, const Vector3D& point);

// Function object for sorting intersections based on the distance from some point p
struct DistanceSorter
{
    DistanceSorter(const Vector3D& p) : m_p(p) {}
    bool operator()(const Intersection& first, const Intersection& second)
    {
        float d1 = Vector3D(first.point() - m_p).lengthSquared();
        float d2 = Vector3D(second.point() - m_p).lengthSquared();
        return d1 < d2;
    }
    const Vector3D& m_p;
};

// Vector operations
Vector3D mirror(const Vector3D& v, const Vector3D& mirror);
#endif
