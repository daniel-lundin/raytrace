#include "utils.h"
#include <cstdlib>
#include <math.h>

// this is just some playing around
Vector3D randomDisplaceVector(const Vector3D& v, float max)
{
    Vector3D dispV(v);    
    float xd, yd, zd;
    xd = (float) rand()/RAND_MAX - .5;
    yd = (float) rand()/RAND_MAX - .5;
    zd = (float) rand()/RAND_MAX - .5;
    xd *= max;
    yd *= max;
    zd *= max;
    dispV.setX(dispV.x() + xd);
    dispV.setY(dispV.y() + yd);
    dispV.setZ(dispV.z() + zd);
    return dispV;
}

Vector3D roundVector(const Vector3D& v)
{
    Vector3D r;
    r.setX(((int) (v.x()*5)) / 10.0);    
    r.setY(((int) (v.y()*5)) / 10.0);    
    r.setZ(((int) (v.z()*5)) / 10.0);    
    return r.normalized();
}

Vector3D sinify(const Vector3D& v, const Vector3D& point)
{
    Vector3D r(v);
    r.setX(r.x() + 0.2*cos(point.x()*10));
    r.setY(r.y() + 0.2*cos(point.y()*10));
    r.setZ(r.z() + 0.2*sin(point.z()*10));
    return r.normalized();
}
