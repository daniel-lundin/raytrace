#include "perlin.h"
#include <math.h>
#include <time.h>

#define PI 3.1416

double cos_interpolation(double a, double b, double x)
{
    double ft = x * PI;
    double f = (1 - cos(ft))*0.5;
    return a*(1-f) + b*f;
}

void Perlin::generate1()
{
    // Generate 10 random numbers between 0 and 1
    srand(1331);
    double rand1[10];
    for(int i=0;i<9;++i)
    {
        rand1[i] = ((rand() % 200) - 100)/100.0;
    }
    rand1[9] = rand1[0];

    for(int i=0;i<1000;++i)
    {
        int aidx = i/100;
        int bidx = (aidx + 1) % 10;
        double x = (i % 100) / 100.0;
        m_noise[i] = cos_interpolation(rand1[aidx], rand1[bidx], x);
    }
}

void Perlin::generate2()
{

}

Vector3D Perlin::displace(const Vector3D& v, const Vector3D& p)
{
    Vector3D retV;
    double amount = .5;
    int xidx = (fabs(p.x())-abs(p.x()))*1000.0;
    int yidx = (fabs(p.y())-abs(p.y()))*1000.0;
    int zidx = (fabs(p.z())-abs(p.z()))*1000.0;
    
    double xoffset = m_noise[xidx % 1000]*amount;
    double yoffset = m_noise[yidx % 1000]*amount;
    double zoffset = m_noise[zidx % 1000]*amount;
    retV.setX(v.x() + xoffset);
    retV.setY(v.y() + yoffset);
    retV.setZ(v.z() + zoffset);
    return retV.normalized();

}
