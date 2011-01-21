#ifndef RAYSPHERE_H
#define RAYSPHERE_H
#include <vector>

#include "rayobject.h"
#include "intersection.h"
#include "vector3d.h"
#include "RayColor.h"
#include "raymaterial.h"


class RaySphere : public RayObject
{
public:
    RaySphere(const Vector3D& center, float radius, const RayMaterial&);

    bool intersects(const Vector3D& start, const Vector3D& direction, std::vector<Intersection>&);
    RayMaterial material();

    // Setters
    void setCenter(const Vector3D& center);
    void setRadius(float radius);
private:
    Vector3D m_center;
    float m_radius;
    RayMaterial m_material;
};

#endif // RAYSPHERE_H
