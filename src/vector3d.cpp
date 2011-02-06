#include "vector3d.h"
#include <math.h>

Vector3D::Vector3D()
    : m_x(0), m_y(0), m_z(0)
{
}
  
Vector3D::Vector3D(double x, double y, double z)
    :m_x(x), m_y(y), m_z(z)
{

}


double Vector3D::x() const
{
    return m_x;
}

double Vector3D::y() const
{
    return m_y;
}

double Vector3D::z() const
{
    return m_z;
}

Vector3D& Vector3D::setX(double x)
{
    m_x = x;
    return *this;
}

Vector3D& Vector3D::setY(double y)
{
    m_y = y;
    return *this;
}

Vector3D& Vector3D::setZ(double z)
{
    m_z = z;
    return *this;
}

// Operations
Vector3D& Vector3D::normalize()
{
    double len = length();
    m_x /= len;
    m_y /= len;
    m_z /= len;

    return *this;
}

double Vector3D::length() const
{
    return sqrt(lengthSquared());
}

double Vector3D::lengthSquared() const
{
    return x()*x() + y()*y() + z()*z();

}



double Vector3D::dotProduct(const Vector3D& other) const
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
    double l = length();
    return Vector3D(x()/l, y()/l, z()/l);
}

const Vector3D Vector3D::operator*(double scale) const
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

std::ostream& operator<<(std::ostream& os, const Vector3D& v)
{
    os << "(" << v.x() << ", " << v.y() << ", " << v.z() << ")";
    return os;
}
