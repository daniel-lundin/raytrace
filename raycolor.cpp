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


RayColor::RayColor(float r, float g, float b)
    : m_r(r), m_g(g), m_b(b)
{

}

void RayColor::setR(float r)
{
    m_r = max(min(r, 255.f), 0.f);
}

void RayColor::setG(float g)
{
    m_g = max(min(g, 255.f), 0.f);
}

void RayColor::setB(float b)
{
    m_b = max(min(b, 255.f), 0.f);
}

float RayColor::r() const
{
    return m_r;
}

float RayColor::g() const
{
    return m_g;
}

float RayColor::b() const
{
    return m_b;
}

void RayColor::scale(float factor)
{
    m_r *= factor;
    m_g *= factor;
    m_b *= factor;
}

RayColor RayColor::scaled(float scale)
{
    RayColor tmp;
    tmp.setR((float)this->r() * scale);
    tmp.setG((float)this->g() * scale);
    tmp.setB((float)this->b() * scale);
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
