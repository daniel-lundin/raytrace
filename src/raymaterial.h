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
    RayMaterial();
    
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
    RayColor m_color;
    float m_ambient;
    float m_diffuse;
    float m_specular;
    float m_specPower;
    float m_reflection;
    float m_refractionRate;
    float m_brytningsIndex;
};

std::ostream& operator<<(std::ostream& os, const RayMaterial&);
#endif // RAYMATERIAL_H
