#include <cstdlib>
#include "raysphere.h"
#include "math.h"
#include "utils.h"

RaySphere::RaySphere(const Vector3D& center, float radius, const RayMaterial& material)
    : m_center(center), m_radius(radius), m_material(material)
{
}


bool RaySphere::intersects(const Vector3D& start, const Vector3D& direction, std::vector<Intersection>& intersections)
{
    // Line = x(t) = o + t*v
    // Sphere = (x - c)(x - c) = r^2

    float A = direction.dotProduct(direction);
    float B = 2*(start - m_center).dotProduct(direction);
    float C = (start - m_center).dotProduct(start - m_center) - m_radius*m_radius;
    float root = B*B - 4*A*C;
    if(root < 0)
        return false;

    // Thw two solutions
    float t1 = (-B + sqrt(root))/(2*A);
    float t2 = (-B - sqrt(root))/(2*A);
    float ts[] = {t1, t2};
    for(int i=0;i<2;++i)
    {
        if(ts[i] < 0)
            continue;
        Intersection inter;
        inter.setPoint(start+direction*ts[i]);
        inter.setObject(this);

        // Normal of itersection
        Vector3D normal = inter.point() - m_center;
        rand();

        normal.normalize();
        // Check if it's a hit from inside by doting direction with normal, if negative, 
        // hit is from inside(used for refractions)
        //if(normal.dotProduct(direction) > 0)
        //    continue;
		inter.setInsideHit(normal.dotProduct(direction) > 0);

        //if(normal.dotProduct(direction) > 0)
        //    continue;

        // Try some random normal displacments
        //inter.setNormal(randomDisplaceVector(normal, 0.01));
        inter.setNormal(normal);

        inter.setMaterial(m_material);
        intersections.push_back(inter);
    }
    return intersections.size() > 0;
}

RayMaterial RaySphere::material()
{
    return m_material;
}

void RaySphere::setCenter(const Vector3D& center)
{
    m_center = center;
}

void RaySphere::setRadius(float radius)
{
    m_radius = radius;
}
