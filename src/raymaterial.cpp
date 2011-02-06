#include "raymaterial.h"

using std::endl;
RayMaterial::RayMaterial(const RayColor& color,
                         double ambient,
                         double diffuse,
                         double specular,
                         double specPower,
                         double reflection,
						 double refraction,
						 double refractionIndex)
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

void RayMaterial::setAmbient(double val)
{
	m_ambient = val;
}

void RayMaterial::setDiffuse(double val)
{
	m_diffuse = val;
}

void RayMaterial::setSpecular(double val)
{
	m_specular = val;
}

void RayMaterial::setSpecPower(double val)
{
	m_specPower = val;
}

void RayMaterial::setReflection(double val)
{
	m_reflection = val;
}

void RayMaterial::setRefraction(double val)
{
	m_refraction = val;
}
void RayMaterial::setRefractionIndex(double val)
{
	m_refractionIndex = val;
}

double RayMaterial::ambient() const
{
    return m_ambient;
}

double RayMaterial::diffuse() const
{
    return m_diffuse;
}

double RayMaterial::specular() const
{
    return m_specular;
}

double RayMaterial::specPower() const
{
    return m_specPower;
}


double RayMaterial::reflection() const
{
    return m_reflection;
}


double RayMaterial::refraction() const
{
	return m_refraction;
}
double RayMaterial::refractionIndex() const
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

