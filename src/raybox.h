#ifndef RAYBOX_H
#define RAYBOX_H

#include "rayobject.h"
#include "raymaterial.h"

// Class represeting a box with it's center
// in (0,0,0)
class RayBox : public RayObject
{
public:
    RayBox(float x, float x, float y, const RayMaterial& m);
    bool intersects(const Vector3D& start,
                    const Vector3D& end,
                    std::vector<Intersection>&);
private:
    float m_x;
    float m_y;
    float m_z;
    RayMaterial m_material;
};
#endif
