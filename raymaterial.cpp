#include "raymaterial.h"

RayMaterial::RayMaterial(const RayColor& color,
                         float ambient,
                         float diffuse,
                         float specular,
                         float specPower,
                         float reflection)
    : m_color(color),
      m_ambient(ambient),
      m_diffuse(diffuse),
      m_specular(specular),
      m_specPower(specPower),
      m_reflection(reflection)
{
}



float RayMaterial::ambient() const
{
    return m_ambient;
}

float RayMaterial::diffuse() const
{
    return m_diffuse;
}

float RayMaterial::specular() const
{
    return m_specular;
}

float RayMaterial::specPower() const
{
    return m_specPower;
}


float RayMaterial::reflection() const
{
    return m_reflection;
}

float RayMaterial::refractionRate() const
{
    return m_refractionRate;
}

float RayMaterial::brytningsIndex() const
{
    return m_brytningsIndex;
}

const RayColor& RayMaterial::color() const
{
    return m_color;
}
