#include "vector3d.h"
#include <math.h>

Vector3D::Vector3D()
    : m_x(0), m_y(0), m_z(0)
{
}

Vector3D::Vector3D(float x, float y, float z)
    :m_x(x), m_y(y), m_z(z)
{

}


float Vector3D::x() const
{
    return m_x;
}

float Vector3D::y() const
{
    return m_y;
}

float Vector3D::z() const
{
    return m_z;
}

Vector3D& Vector3D::setX(float x)
{
    m_x = x;
    return *this;
}

Vector3D& Vector3D::setY(float y)
{
    m_y = y;
    return *this;
}

Vector3D& Vector3D::setZ(float z)
{
    m_z = z;
    return *this;
}

// Operations
Vector3D& Vector3D::normalize()
{
    float len = length();
    m_x /= len;
    m_y /= len;
    m_z /= len;

    return *this;
}

float Vector3D::length() const
{
    return sqrt(lengthSquared());
}

float Vector3D::lengthSquared() const
{
    return x()*x() + y()*y() + z()*z();

}



float Vector3D::dotProduct(const Vector3D& other) const
{
    return x()*other.x() + y()*other.y() + z()*other.z();
}
Vector3D Vector3D::crossProduct(const Vector3D& other) const
{    
    return Vector3D(y()*other.z() - z()*other.y(),
                    z()*other.x() - x()*other.z(),
                    x()*other.y() - y()*other.x());
}

Vector3D Vector3D::normalized() const
{
    float l = length();
    return Vector3D(x()/l, y()/l, z()/l);
}

const Vector3D Vector3D::operator*(float scale) const
{
    return Vector3D(x()*scale, y()*scale, z()*scale);
}

const Vector3D Vector3D::operator+(const Vector3D& other) const
{
    return Vector3D(x() + other.x(), y() + other.y(), z() + other.z());
}

const Vector3D Vector3D::operator-(const Vector3D& other) const
{
    return Vector3D(x() - other.x(), y() - other.y(), z() - other.z());
}
