#include "checkertexture.h"
#include "intersection.h"
#include <vector>
#include <math.h>

CheckerTexture::CheckerTexture(RayObject* object, 
                               const RayColor& c1,
                               const RayColor& c2)
    : m_object(object), m_c1(c1), m_c2(c2)
{
}

bool CheckerTexture::intersects(const Vector3D& start, 
                                const Vector3D& direction, 
                                std::vector<Intersection>& isecs)
{
    if(!m_object->intersects(start, direction, isecs))
        return false;

    std::vector<Intersection>::iterator it = isecs.begin();
    std::vector<Intersection>::iterator end = isecs.end();
    while(it != end)
    {
        if((lround(it->point().x()) + lround(it->point().z()))%2)
            it->material().setColor(m_c1);
        else
            it->material().setColor(m_c2);
        ++it;
    }
    return true;
}
