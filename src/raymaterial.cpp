#include "raymaterial.h"

using std::endl;
RayMaterial::RayMaterial(const RayColor& color,
                         float ambient,
                         float diffuse,
                         float specular,
                         float specPower,
                         float reflection,
						 float refraction,
						 float refractionIndex)
    : m_color(color),
      m_ambient(ambient),
      m_diffuse(diffuse),
      m_specular(specular),
      m_specPower(specPower),
      m_reflection(reflection),
	  m_refraction(refraction),
	  m_refractionIndex(refractionIndex)
{
}


RayMaterial::RayMaterial()
    : m_color(0,0,0),
      m_ambient(0),
      m_diffuse(0),
      m_specular(0),
      m_specPower(0),
      m_reflection(0)
{
}

void RayMaterial::setColor(const RayColor& color)
{
	m_color = color;
}

void RayMaterial::setAmbient(float val)
{
	m_ambient = val;
}

void RayMaterial::setDiffuse(float val)
{
	m_diffuse = val;
}

void RayMaterial::setSpecular(float val)
{
	m_specular = val;
}

void RayMaterial::setSpecPower(float val)
{
	m_specPower = val;
}

void RayMaterial::setReflection(float val)
{
	m_reflection = val;
}

void RayMaterial::setRefraction(float val)
{
	m_refraction = val;
}
void RayMaterial::setRefractionIndex(float val)
{
	m_refractionIndex = val;
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


float RayMaterial::refraction() const
{
	return m_refraction;
}
float RayMaterial::refractionIndex() const
{
    return m_refractionIndex;
}

const RayColor& RayMaterial::color() const
{
    return m_color;
}

std::ostream& operator<<(std::ostream& os, const RayMaterial& mat)
{
    os << " .. RayMaterial .. " << endl;    
    os << "Color: " << mat.m_color.toString() << endl;
        
    os << "Ambient: " << mat.m_ambient << endl;
    os << "Diffuse: " << mat.m_diffuse << endl;
    os << "specular: " << mat.m_specular << endl;
    os << "specPower:" << mat.m_specPower << endl;
    os << "reflection: " << mat.m_reflection << endl;
    os << "refractionIndex: " << mat.m_refractionIndex << endl;
    return os;
}

