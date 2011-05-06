#ifndef _PERLIN_H
#define _PERLIN_H
#include "vector3d.h"

class Perlin
{
public:
    void generate1(int seed=0);
    void generate2();

    Vector3D displace(const Vector3D& p, const Vector3D& v, double intensity, double scale);
private:
    Vector3D m_gradients[1000];
};

#endif
