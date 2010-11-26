#include "intersection.h"


Intersection::Intersection()
    : m_insideHit(true)
{

}


const Vector3D& Intersection::point()
{
    return m_point;
}

const Vector3D& Intersection::normal()
{
    return m_normal;
}

const RayColor Intersection::color()
{
    return m_color;
}

RayObject* Intersection::object()
{
    return m_object;
}

bool Intersection::insideHit()
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

void Intersection::setColor(const RayColor& color)
{
    m_color = color;
}

void Intersection::setObject(RayObject* object)
{
    m_object = object;
}

void Intersection::setInsideHit(bool b)
{
    m_insideHit = b;
}
