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
                float reflection,
				float refraction,
				float refractionIndex);
    RayMaterial();
    
	// setters
	void setColor(const RayColor&);
    void setAmbient(float);
    void setDiffuse(float);
    void setSpecular(float);
    void setSpecPower(float);
    void setReflection(float);
	void setRefraction(float);
    void setRefractionIndex(float);

	// getters
    const RayColor& color() const;
    float ambient() const;
    float diffuse() const;
    float specular() const;
    float specPower() const;
    float reflection() const;
	float refraction() const;
    float refractionIndex() const;


    friend std::ostream& operator<<(std::ostream& os, const RayMaterial&);
private:
    RayColor m_color;
    float m_ambient;
    float m_diffuse;
    float m_specular;
    float m_specPower;
    float m_reflection;
	float m_refraction;
    float m_refractionIndex;
};

std::ostream& operator<<(std::ostream& os, const RayMaterial&);
#endif // RAYMATERIAL_H
