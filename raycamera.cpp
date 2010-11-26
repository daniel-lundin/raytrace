#include "raycamera.h"

RayCamera::RayCamera()
{
}

const Vector3D& RayCamera::location()
{
    return m_location;
}

const Vector3D& RayCamera::up()
{
    return m_up;
}

const Vector3D& RayCamera::lookat()
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
