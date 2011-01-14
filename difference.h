#ifndef DIFFERENCE_H
#define DIFFERENCE_H

#include "rayobject.h"
#include "intersection.h"

class Difference : public RayObject
{
public:
    Difference(RayObject* first, RayObject* second);

    bool intersects(const Vector3D& start, const Vector3D& direction, std::vector<Intersection>&);
private:
    RayObject* m_first;
    RayObject* m_second;
};
#endif
