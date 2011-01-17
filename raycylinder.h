#ifndef RAYCYLINDER_H
#define RAYCYLINDER_H
#include <vector>
#include "rayobject.h"
#include "intersection.h"

class RayCylinder : public RayObject
{
public:
    RayCylinder(float radius);

    bool intersects(const Vector3D& start, 
                    const Vector3D& direction, 
                    std::vector<Intersection>&);
private:
    float m_radius;
};
#endif
