#ifndef ROTATION_H
#define ROTATION_H
#include "rayobject.h"

class Rotation : public RayObject
{
public:
    Rotation(RayObject* obj, float xrot, float yrot, float zrot);

    bool intersects(const Vector3D& start, 
                    const Vector3D& direction, 
                    std::vector<Intersection>&);

private:
    RayObject* m_obj;
    float m_xrot;
    float m_yrot;
    float m_zrot;
};

#endif
