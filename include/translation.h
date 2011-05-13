#ifndef TRANSLATION_H
#define TRANSLATION_H
#include "rayobject.h"

// Class for translating an object
class Translation : public RayObject
{
public:
    Translation(RayObject* obj, double x, double y, double z);

    
    bool intersects(const Vector3D& start, 
                    const Vector3D& direction, 
                    std::vector<Intersection>&);
private:
    RayObject* m_obj;

    double m_x;
    double m_y;
    double m_z;
};

#endif
