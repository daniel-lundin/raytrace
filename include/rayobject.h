#ifndef RAYOBJECT_H
#define RAYOBJECT_H
#include "vector3d.h"
#include "intersection.h"
#include "raycolor.h"
#include "raymaterial.h"
#include <vector>

class RayObject
{
public:
    virtual bool intersects(const Vector3D& start, 
                            const Vector3D& direction, 
                            std::vector<Intersection>&) = 0;
};

#endif // RAYOBJECT_H
