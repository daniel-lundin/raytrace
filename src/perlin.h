#ifndef _PERLIN_H
#define _PERLIN_H
#include "vector3d.h"

class Perlin
{
public:
    void generate1();
    void generate2();

    Vector3D displace(const Vector3D& v, const Vector3D& p);
private:
    double m_noise[1000];
};

#endif
