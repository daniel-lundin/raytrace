#ifndef PHOTONINTERSECTION_H
#define PHOTONINTERSECTION_H
#include "vector3d.h"
#include "raycolor.h"

// For now 3DTree only
// Todo: Template argument for dimension(also for datatype)
struct PhotonIntersection {
    PhotonIntersection(const Vector3D& pos, 
                       const Vector3D& dir, 
                       const RayColor& col)
        : position(pos), incomingDirection(dir), color(col)
    { }
    Vector3D position;
    Vector3D incomingDirection;
    RayColor color;
};

#endif
