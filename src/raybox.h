#ifndef RAYBOX_H
#define RAYBOX_H

#include "rayobject.h"
#include "raymaterial.h"

// Class represeting a box with it's center
// in (0,0,0)
class RayBox : public RayObject
{
public:
    RayBox(double x, double x, double y, const RayMaterial& m);
    bool intersects(const Vector3D& start,
                    const Vector3D& end,
                    std::vector<Intersection>&);
private:
    double m_x;
    double m_y;
    double m_z;
    RayMaterial m_material;
};
#endif
