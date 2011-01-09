#ifndef RAYMATERIAL_H
#define RAYMATERIAL_H
#include <iostream>
#include "raycolor.h"

class RayMaterial
{
public:
    RayMaterial(const RayColor& color,
                float ambient,
                float diffuse,
                float specular,
                float specPower,
                float reflection);

    float ambient() const;
    float diffuse() const;
    float specular() const;
    float specPower() const;
    float reflection() const;
    float refractionRate() const;
    float brytningsIndex() const;
    const RayColor& color() const;


    friend std::ostream& operator<<(std::ostream& os, const RayMaterial&);
private:
    float m_ambient;
    float m_diffuse;
    float m_specular;
    float m_specPower;
    float m_reflection;
    float m_refractionRate;
    float m_brytningsIndex;
    RayColor m_color;
};

std::ostream& operator<<(std::ostream& os, const RayMaterial&);
#endif // RAYMATERIAL_H
