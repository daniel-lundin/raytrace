#include "pointlight.h"

PointLight::PointLight(const Vector3D& position)
    : m_position(position)
{
}


const Vector3D& PointLight::position() const
{
    return m_position;
}
