#ifndef RAYMATERIAL_H
#define RAYMATERIAL_H
#include "raycolor.h"

class RayMaterial
{
public:
    RayMaterial(float reflection, const RayColor& color, float refractionRate = 0, float brytningsIndex = 1);

    float ambient();
    float specular();
    float specPower();
    float reflectionRate();
    float refractionRate();
    float brytningsIndex();
    RayColor& color();
private:
    float m_ambient;
    float m_specular;
    float m_specPower;
    float m_reflectionRate;
    float m_refractionRate;
    float m_brytningsIndex;
    RayColor m_color;
};

#endif // RAYMATERIAL_H
