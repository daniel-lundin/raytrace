#ifndef CHECKTEXTURE_H
#define CHECKTEXTURE_H

#include "rayobject.h"

class CheckerTexture: public RayObject
{
public:
    CheckerTexture(RayObject* obj);
    bool intersects(const Vector3D& start, 
                    const Vector3D& direction, 
                    std::vector<Intersection>&);
private:
    RayObject* m_object;

};
#endif
