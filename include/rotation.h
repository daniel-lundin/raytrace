#ifndef ROTATION_H
#define ROTATION_H
#include "rayobject.h"

// Forward declarations
class Vector3D;

class Rotation : public RayObject
{
public:
    Rotation(RayObject* obj, double xrot, double yrot, double zrot);
    Rotation(RayObject* obj, const Vector3D&);

    bool intersects(const Vector3D& start, 
                    const Vector3D& direction, 
                    std::vector<Intersection>&);

private:
    RayObject* m_obj;
    double m_xrot;
    double m_yrot;
    double m_zrot;
};

#endif
