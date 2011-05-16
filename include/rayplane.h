#ifndef RAYPLANE_H
#define RAYPLANE_H
#include <vector>

#include "raycolor.h"
#include "vector3d.h"
#include "rayobject.h"
#include "raymaterial.h"
#include "perlin.h"

class RayPlane : public RayObject
{
public:
    RayPlane() {}
    RayPlane(const Vector3D& point, const Vector3D& up, const RayMaterial& color);

    bool intersects(const Vector3D& start, const Vector3D& direction, std::vector<Intersection>&);
    Vector3D m_up;
private:

    Vector3D m_point;
    RayMaterial m_material;
    Perlin m_perlin;
};

#endif // RAYPLANE_H
