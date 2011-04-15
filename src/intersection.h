#ifndef INTERSECTION_H
#define INTERSECTION_H
#include "vector3d.h"
#include "raycolor.h"
#include "raymaterial.h"
// Forward declarations
class RayObject;

class Intersection
{
public:
    Intersection();    

    const Vector3D& point() const;
    const Vector3D& normal() const;
    RayObject* object();
    RayMaterial& material();
    bool insideHit() const;

    void setPoint(const Vector3D& point);
    void setNormal(const Vector3D& normal);
    void setObject(RayObject*);
    void setInsideHit(bool b);
    void setMaterial(const RayMaterial&);
private:
    RayObject* m_object;
    RayMaterial m_material;
    Vector3D m_point;
    Vector3D m_normal;
    bool m_insideHit;
};

#endif // INTERSECTION_H
