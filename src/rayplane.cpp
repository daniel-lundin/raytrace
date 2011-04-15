#include "rayplane.h"
#include "utils.h"
#include "perlin.h"

RayPlane::RayPlane(const Vector3D& point, const Vector3D& up, const RayMaterial& material)
    : m_up(up), m_point(point), m_material(material)
{
    m_up.normalize();
    //m_perlin.generate1();
}



bool RayPlane::intersects(const Vector3D& start, const Vector3D& direction, std::vector<Intersection>& intersections)
{
    double l = (m_up.x()*m_point.x() + m_up.y()*m_point.y() + m_up.z()*m_point.z());
    l -= (m_up.x() * start.x() + m_up.y()*start.y() + m_up.z()*start.z());
    l /= (m_up.x()*direction.x() + m_up.y()*direction.y() + m_up.z()*direction.z());

    if(l<0)
        return false;
    Intersection i;
    i.setObject(this);
    i.setPoint(start + direction*l);
    i.setNormal(m_up);
    //i.setNormal(m_perlin.displace(m_up, i.point()));//randomDisplaceVector(m_up, 0.1));
    i.setInsideHit(direction.dotProduct(m_up) < 0);

	// Hardcoded checker
	//int shade = (((int) i.point().x() + (int) i.point().z())) & 1;
    //shade *= 255;
	//m_material.setColor(RayColor(shade, shade, shade));

    i.setMaterial(m_material);
    intersections.push_back(i);
    return true;
}
