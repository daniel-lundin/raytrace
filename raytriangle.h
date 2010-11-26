#ifndef RAYTRIANGLE_H
#define RAYTRIANGLE_H
#include "vector3d.h"

#include "rayobject.h"
#include "raymaterial.h"

class RayPlane;

class RayTriangle : public RayObject
{
public:
    RayTriangle(const Vector3D& v1, const Vector3D& v2, const Vector3D& v3, const RayMaterial&);
    ~RayTriangle();

    bool intersects(const Vector3D& start, const Vector3D& direction, QList<Intersection>&);
    RayMaterial material();

private:
    Vector3D m_v1;
    Vector3D m_v2;
    Vector3D m_v3;
    RayMaterial m_material;
    RayPlane* m_plane;
};

#endif // RAYTRIANGLE_H
