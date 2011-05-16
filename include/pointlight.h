#ifndef POINTLIGHT_H
#define POINTLIGHT_H
#include "vector3d.h"

class PointLight
{
public:
    PointLight(const Vector3D& position);
    const Vector3D& position() const;
private:
    Vector3D m_position;

};

#endif // POINTLIGHT_H
