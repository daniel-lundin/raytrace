#ifndef _NORMALMAPPER_H
#define _NORMALMAPPER_H

#include "vector3d.h"

class NormalMapper
{
public:
    virtual Vector3D map(const Vector3D& v) = 0;
};
#endif
