#ifndef CHECKTEXTURE_H
#define CHECKTEXTURE_H

#include "rayobject.h"
#include "raycolor.h"

class CheckerTexture: public RayObject
{
public:
    CheckerTexture(RayObject* obj, const RayColor&, const RayColor&);
    bool intersects(const Vector3D& start, 
                    const Vector3D& direction, 
                    std::vector<Intersection>&);
private:
    RayObject* m_object;
    RayColor m_c1;
    RayColor m_c2;

};
#endif
