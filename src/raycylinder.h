#ifndef RAYCYLINDER_H
#define RAYCYLINDER_H
#include <vector>
#include "rayobject.h"
#include "intersection.h"
#include "raymaterial.h"

class RayCylinder : public RayObject
{
public:
    RayCylinder(float radius, float length, const RayMaterial&);

    bool intersects(const Vector3D& start, 
                    const Vector3D& direction, 
                    std::vector<Intersection>&);
private:
    RayMaterial m_material;
    float m_radius;
    float m_length;
};
#endif
