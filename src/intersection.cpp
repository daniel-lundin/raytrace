#include "intersection.h"


Intersection::Intersection()
    : m_insideHit(true)
{

}


const Vector3D& Intersection::point() const
{
    return m_point;
}

const Vector3D& Intersection::normal() const
{
    return m_normal;
}

RayObject* Intersection::object()
{
    return m_object;
}

RayMaterial& Intersection::material()
{
    return m_material;
}

bool Intersection::insideHit() const
{
    return m_insideHit;
}

void Intersection::setPoint(const Vector3D& point)
{
    m_point = point;
}

void Intersection::setNormal(const Vector3D& normal)
{
    m_normal = normal;
}

void Intersection::setObject(RayObject* object)
{
    m_object = object;
}

void Intersection::setMaterial(const RayMaterial& m)
{
    m_material = m;
}

void Intersection::setInsideHit(bool b)
{
    m_insideHit = b;
}
