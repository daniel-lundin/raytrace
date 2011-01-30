#include "raycylinder.h"
#include "raymaterial.h"
#include <math.h>


RayCylinder::RayCylinder(float radius, float length, const RayMaterial& m)
    : m_material(m), m_radius(radius), m_length(length) 
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
    float c = pow(start.x(), 2) + pow(start.y(), 2) - pow(m_radius,2);

    float root = pow(b,2) - 4*a*c;
    if(root < 0)
        return false;
    
    float t1 = (-b + sqrt(root))/(2*a);
    float t2 = (-b - sqrt(root))/(2*a);

    float ts[] = {t1, t2};
    for(int i=0;i<2;++i)
    {
        if(ts[i] < 0)
            continue;
        Vector3D point = start + direction*ts[i];
        if((point.z() > m_length/2) || (point.z() < -m_length/2))
            continue;
        
        Intersection i;
        i.setMaterial(m_material);
        i.setObject(this);
        i.setPoint(point);
        Vector3D normal = i.point() - Vector3D(0, 0, i.point().z());
        i.setInsideHit(normal.dotProduct(direction) > 0);
        i.setNormal(normal.normalized());
        isecs.push_back(i);
    }
    return isecs.size() > 0;

}
