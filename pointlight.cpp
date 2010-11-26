#include "pointlight.h"

PointLight::PointLight(const Vector3D& position)
    : m_position(position)
{
}


const Vector3D& PointLight::position()
{
    return m_position;
}
