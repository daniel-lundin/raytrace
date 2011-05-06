#include "randomnormaldisplacer.h"
#include "utils.h"
#include "perlin.h"

RandomNormalDisplacer::RandomNormalDisplacer(RayObject* obj, double amount)
    : m_object(obj), m_perlin(0), m_amount(amount)
{ 
    m_perlin = new Perlin();
    m_perlin->generate1(100);
}

RandomNormalDisplacer::~RandomNormalDisplacer()
{
    delete m_perlin;
}

bool RandomNormalDisplacer::intersects(const Vector3D& start, 
                                       const Vector3D& direction, 
                                       std::vector<Intersection>& isecs)
{
    if(!m_object->intersects(start, direction, isecs))
        return false;
    std::vector<Intersection>::iterator it = isecs.begin();
    std::vector<Intersection>::iterator end = isecs.end();
    while(it != end)
    {
        it->setNormal(m_perlin->displace(it->point(), it->normal(), .2, 0.3));
        ++it;
    }
    return true;
}
