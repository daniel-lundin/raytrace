#include <utility>
#include <algorithm>
#include <sstream>
#include "raycolor.h"

using std::min;
using std::max;
using std::ostringstream;

RayColor::RayColor()
    :m_r(0), m_g(0), m_b(0)
{
}


RayColor::RayColor(double r, double g, double b)
    : m_r(r), m_g(g), m_b(b)
{

}

void RayColor::setR(double r)
{
    m_r = max(min(r, 255.0), 0.0);
}

void RayColor::setG(double g)
{
    m_g = max(min(g, 255.0), 0.0);
}

void RayColor::setB(double b)
{
    m_b = max(min(b, 255.0), 0.0);
}

double RayColor::r() const
{
    return m_r;
}

double RayColor::g() const
{
    return m_g;
}

double RayColor::b() const
{
    return m_b;
}

void RayColor::scale(double factor)
{
    m_r *= factor;
    m_g *= factor;
    m_b *= factor;
}

RayColor RayColor::scaled(double scale)
{
    RayColor tmp;
    tmp.setR((double)this->r() * scale);
    tmp.setG((double)this->g() * scale);
    tmp.setB((double)this->b() * scale);
    return tmp;
}

// Operators
RayColor RayColor::operator+(const RayColor& other)
{
    RayColor tmp(*this);
    tmp += other;
    return tmp;
}

RayColor& RayColor::operator+=(const RayColor& other)
{
    this->setR(r() + other.r());
    this->setG(g() + other.g());
    this->setB(b() + other.b());
    return *this;
}
std::string RayColor::toString() const
{
    ostringstream oss;
    oss << "(" << m_r << ", " << m_g << ", " << m_b << ")";
    return oss.str();
}
