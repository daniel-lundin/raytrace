#include "utils.h"
#include <cstdlib>
#include <math.h>

// this is just some playing around
Vector3D randomDisplaceVector(const Vector3D& v, double max)
{
    Vector3D dispV(v);
    double xd, yd, zd;
    xd = (double) rand()/RAND_MAX - .5;
    yd = (double) rand()/RAND_MAX - .5;
    zd = (double) rand()/RAND_MAX - .5;
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

Vector3D mirror(const Vector3D& v, const Vector3D& mirror)
{
	return v - mirror*mirror.dotProduct(v)*2;
}

Vector3D refract(const Vector3D& in, const Vector3D& normal, double n1, double n2)
{
	double cosin = in.dotProduct(normal * -1);
	double cosout = sqrt(1 - pow((n1/n2),2)*(1 - pow(cosin, 2)));
	Vector3D refract;
	if (cosin > 0) 
	{
		refract = in*(n1/n2) + normal*((n1/n2)*cosin - cosout);
	}
	else
	{
		refract = in*(n1/n2) + normal*((n1/n2)*cosin + cosout);
	}
	return refract;
}
