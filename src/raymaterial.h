#ifndef RAYMATERIAL_H
#define RAYMATERIAL_H
#include <iostream>
#include "raycolor.h"

class RayMaterial
{
public:
    RayMaterial(const RayColor& color,
                double ambient,
                double diffuse,
                double specular,
                double specPower,
                double reflection,
				double refraction,
				double refractionIndex);
    RayMaterial();
    
	// setters
	void setColor(const RayColor&);
    void setAmbient(double);
    void setDiffuse(double);
    void setSpecular(double);
    void setSpecPower(double);
    void setReflection(double);
	void setRefraction(double);
    void setRefractionIndex(double);

	// getters
    const RayColor& color() const;
    double ambient() const;
    double diffuse() const;
    double specular() const;
    double specPower() const;
    double reflection() const;
	double refraction() const;
    double refractionIndex() const;


    friend std::ostream& operator<<(std::ostream& os, const RayMaterial&);
private:
    RayColor m_color;
    double m_ambient;
    double m_diffuse;
    double m_specular;
    double m_specPower;
    double m_reflection;
	double m_refraction;
    double m_refractionIndex;
};

std::ostream& operator<<(std::ostream& os, const RayMaterial&);
#endif // RAYMATERIAL_H
