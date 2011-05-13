#ifndef RAYSPHERE_H
#define RAYSPHERE_H
#include <vector>

#include "rayobject.h"
#include "intersection.h"
#include "vector3d.h"
#include "raycolor.h"
#include "raymaterial.h"
#include "perlin.h"


class RaySphere : public RayObject
{
public:
    RaySphere(const Vector3D& center, double radius, const RayMaterial&);

    bool intersects(const Vector3D& start, const Vector3D& direction, std::vector<Intersection>&);
    RayMaterial material();

    // Setters
    void setCenter(const Vector3D& center);
    void setRadius(double radius);
private:
    Vector3D m_center;
    double m_radius;
    RayMaterial m_material;
    Perlin m_perlin;
};

#endif // RAYSPHERE_H
