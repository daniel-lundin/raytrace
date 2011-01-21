#include "raycamera.h"

RayCamera::RayCamera()
{
}

RayCamera::RayCamera(const Vector3D& location, const Vector3D& lookat, const Vector3D& up)
    : m_location(location), m_lookat(lookat), m_up(up)
{
}

const Vector3D& RayCamera::location() const
{
    return m_location;
}

const Vector3D& RayCamera::up() const
{
    return m_up;
}

const Vector3D& RayCamera::lookat() const
{
    return m_lookat;
}

void RayCamera::setLocation(const Vector3D& location)
{
    m_location = location;
}

void RayCamera::setUp(const Vector3D& up)
{
    m_up = up;
}

void RayCamera::setLookat(const Vector3D& lookat)
{
    m_lookat = lookat;
}
