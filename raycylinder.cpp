#include "raycylinder.h"
#include "raymaterial.h"
#include <math.h>


RayCylinder::RayCylinder(float radius)
    : m_radius(radius)
{
}

bool RayCylinder::intersects(const Vector3D& start, 
                             const Vector3D& direction, 
                             std::vector<Intersection>& isecs)
{
    // ray: x(t) = start + t*dir
    // cylinder: x^2 + y^2 = r^2
    
    // t = -b +- sqrt(b^2-4ac) / 2a
    // where:
    // a = dir_x^2 + dir_y^2
    // b = 2*start_x*dir_x + 2*start_y*dir_y
    // c = start_x^2 + start_y^2 - radius

    float a = pow(direction.x(), 2) + pow(direction.y(), 2);
    float b = 2*start.x()*direction.x() + 2*start.y()*direction.y();
    float c = pow(start.x(), 2) + pow(start.y(), 2) - m_radius;

    float root = pow(b,2) - 4*a*c;
    if(root < 0)
        return false;
    
    float t1 = (-b + sqrt(root))/(2*a);
    float t2 = (-b - sqrt(root))/(2*a);

    if(t1 > 0)
    {
        Intersection i;
        i.setMaterial(RayMaterial(RayColor(255,0,255), 0.5, 0.5, 0, 0, 0));
        i.setObject(this);
        i.setPoint(start + direction*t1);
        i.setNormal(i.point() - Vector3D(0, 0, i.point().z()));
        isecs.push_back(i);
    }
    if(t2 > 0)
    {
        Intersection i;
        i.setMaterial(RayMaterial(RayColor(255,0,255), 0.5, 0.5, 0, 0, 0));
        i.setObject(this);
        i.setPoint(start + direction*t2);
        i.setNormal(i.point() - Vector3D(0, 0, i.point().z()));
        isecs.push_back(i);
    }
    return isecs.size() > 0;

}
