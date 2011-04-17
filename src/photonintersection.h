#ifndef PHOTONINTERSECTION_H
#define PHOTONINTERSECTION_H
#include "vector3d.h"

// For now 3DTree only
// Todo: Template argument for dimension(also for datatype)
struct PhotonIntersection {
    PhotonIntersection(const Vector3D& pos, const Vector3D& dir)
        : position(pos), incomingDirection(dir)
    { }
    Vector3D position;
    Vector3D incomingDirection;
};

#endif
