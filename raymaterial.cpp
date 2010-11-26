#include "raymaterial.h"

RayMaterial::RayMaterial(float reflection, const RayColor& color, float refractionRate, float brytningsIndex)
    : m_reflectionRate(reflection), m_color(color), m_refractionRate(refractionRate), m_brytningsIndex(brytningsIndex)
{
}


float RayMaterial::reflectionRate()
{
    return m_reflectionRate;
}

float RayMaterial::refractionRate()
{
    return m_refractionRate;
}

float RayMaterial::brytningsIndex()
{
    return m_brytningsIndex;
}

RayColor& RayMaterial::color()
{
    return m_color;
}
