#include "checkertexture.h"
#include "intersection.h"
#include <vector>
#include <math.h>

CheckerTexture::CheckerTexture(RayObject* object)
    : m_object(object)
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
            it->material().setColor(RayColor(0,0,0));
        else
            it->material().setColor(RayColor(255,255,255));
        ++it;
    }
    return true;
}
