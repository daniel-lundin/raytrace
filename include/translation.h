#ifndef TRANSLATION_H
#define TRANSLATION_H
#include "rayobject.h"

// Forward declarations
class Vector3D;

/// Class for translating an object
class Translation : public RayObject
{
public:
    Translation(RayObject* obj, double x, double y, double z);
    Translation(RayObject* obj, const Vector3D&);
    
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
