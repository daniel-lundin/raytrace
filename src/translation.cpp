#include "translation.h"

Translation::Translation(RayObject* obj, float x, float y, float z)
    : m_obj(obj), m_x(x), m_y(y), m_z(z)
{
}

bool Translation::intersects(const Vector3D& start, 
                            const Vector3D& direction, 
                            std::vector<Intersection>& isecs)
{

    // Instead of transforming the object, we take the inverse transform
    // and apply it to the ray

    Vector3D start_t = start + Vector3D(-m_x, -m_y, -m_z);
    if(!m_obj->intersects(start_t, direction, isecs))
        return false;
    
    // Iterate through intersection and translate intersection points
    std::vector<Intersection>::iterator it = isecs.begin();
    std::vector<Intersection>::iterator end = isecs.end();
    for(;it != end; ++it)
    {
        it->setObject(this);
        it->setPoint(it->point() - Vector3D(-m_x, -m_y, -m_z));
    }
    return true;
}

