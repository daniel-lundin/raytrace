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
    return m_x*m_x + m_y*m_y + m_z*m_z;
}

double Vector3D::distanceFrom(const Vector3D& p) const
{
    return sqrt(Vector3D::distanceSquared(*this, p));
}
double Vector3D::distance(const Vector3D& v1, const Vector3D& v2)
{
    return sqrt(Vector3D::distanceSquared(v1, v2));
}

double Vector3D::distanceSquared(const Vector3D& v1, const Vector3D& v2)
{
    return pow(v2.m_x - v1.m_x, 2) + 
           pow(v2.m_y - v1.m_y, 2) + 
           pow(v2.m_z - v1.m_z, 2); 
}

double Vector3D::dotProduct(const Vector3D& other) const
{
    return m_x*other.m_x + m_y*other.m_y + m_z*other.m_z;
}

Vector3D Vector3D::crossProduct(const Vector3D& other) const
{    
    return Vector3D(m_y*other.m_z - m_z*other.m_y,
                    m_z*other.m_x - m_x*other.m_z,
                    m_x*other.m_y - m_y*other.m_x);
}

Vector3D Vector3D::normalized() const
{
    double l = length();
    return Vector3D(m_x/l, m_y/l, m_z/l);
}

Vector3D Vector3D::scaled(double factor) const
{
    return Vector3D(m_x*factor, m_y*factor, m_z*factor);
}

const Vector3D Vector3D::operator*(double scale) const
{
    return Vector3D(m_x*scale, m_y*scale, m_z*scale);
}

const Vector3D Vector3D::operator+(const Vector3D& other) const
{
    return Vector3D(m_x + other.m_x, m_y + other.m_y, m_z + other.m_z);
}

const Vector3D Vector3D::operator-(const Vector3D& other) const
{
    return Vector3D(m_x - other.m_x, m_y - other.m_y, m_z - other.m_z);
}

std::ostream& operator<<(std::ostream& os, const Vector3D& v)
{
    os << "(" << v.x() << ", " << v.y() << ", " << v.z() << ")";
    return os;
}
