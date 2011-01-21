#ifndef TRANSLATION_H
#define TRANSLATION_H
#include "rayobject.h"

// Class for translating an object
class Translation : public RayObject
{
public:
    Translation(RayObject* obj, float x, float y, float z);

    
    bool intersects(const Vector3D& start, 
                    const Vector3D& direction, 
                    std::vector<Intersection>&);
private:
    float m_x;
    float m_y;
    float m_z;

    RayObject* m_obj;
};

#endif
