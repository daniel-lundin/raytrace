#include "randomnormaldisplacer.h"
#include "utils.h"

RandomNormalDisplacer::RandomNormalDisplacer(RayObject* obj, double amount)
    : m_object(obj), m_amount(amount)
{ }

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
        it->setNormal(randomDisplaceVector(it->normal(), m_amount));
        ++it;
    }
    return true;
}
