#ifndef RAYOBJECT_H
#define RAYOBJECT_H
#include "vector3d.h"
#include "intersection.h"
#include "RayColor.h"
#include "raymaterial.h"
#include <QList>
class RayObject
{
public:
    virtual bool intersects(const Vector3D& start, const Vector3D& direction, QList<Intersection>&) = 0;
    virtual RayMaterial material() = 0;
};

#endif // RAYOBJECT_H
