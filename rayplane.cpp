#include "rayplane.h"

RayPlane::RayPlane(const Vector3D& point, const Vector3D& up, const RayMaterial& material)
    : m_point(point), m_up(up), m_material(material)
{
    m_up.normalize();
}



bool RayPlane::intersects(const Vector3D& start, const Vector3D& direction, std::vector<Intersection>& intersections)
{
    float l = (m_up.x()*m_point.x() + m_up.y()*m_point.y() + m_up.z()*m_point.z());
    l -= (m_up.x() * start.x() + m_up.y()*start.y() + m_up.z()*start.z());
    l /= (m_up.x()*direction.x() + m_up.y()*direction.y() + m_up.z()*direction.z());

    if(l<0)
        return false;
    Intersection i;
    i.setObject(this);
    i.setPoint(start + direction*l);
    i.setNormal(m_up);
    i.setInsideHit(direction.dotProduct(m_up) < 0);

    //int colorFlag = ((int)i.point().x() + (int)i.point().z()) % 2 == 0;

    //i.setColor(RayColor(colorFlag*255,colorFlag*255,colorFlag*255));
    i.setColor(m_material.color());
    intersections.push_back(i);
    return true;
}


RayMaterial RayPlane::material()
{
    return m_material;
}

