#ifndef INTERSECTION_H
#define INTERSECTION_H
#include "vector3d.h"
#include "raycolor.h"
// Forward declarations
class RayObject;

class Intersection
{
public:
    Intersection();    

    const Vector3D& point();
    const Vector3D& normal();
    const RayColor color();
    RayObject* object();
    bool insideHit();

    void setPoint(const Vector3D& point);
    void setNormal(const Vector3D& normal);
    void setColor(const RayColor& color);
    void setObject(RayObject*);
    void setInsideHit(bool b);
private:
    RayObject* m_object;
    Vector3D m_point;
    Vector3D m_normal;
    RayColor m_color;
    bool m_insideHit;
};

#endif // INTERSECTION_H
