#include "perlin.h"
#include <math.h>
#include <iostream>
#include <time.h>

using namespace std;
#define PI 3.1416

double cos_interpolation(double a, double b, double x)
{
    double ft = x * PI;
    double f = (1 - cos(ft))*0.5;
    return a*(1-f) + b*f;
}

void Perlin::generate1(int seed)
{
    for(int i=0;i<1000;++i)
    {
        srand(seed + i);
        m_gradients[i] = Vector3D(((rand() % 200) - 100)/100.0,
                                  ((rand() % 200) - 100)/100.0,
                                  ((rand() % 200) - 100)/100.0);
    }
}

void Perlin::generate2()
{

}

Vector3D Perlin::displace(const Vector3D& p, const Vector3D& v, double intensity, double scale)
{
    Vector3D dispV;
    // Scale vector control intesity of displacment
    Vector3D _p = p.scaled(intensity);

    // Round components
    int floorX = (int)floor(_p.x());
    int floorY = (int)floor(_p.y());
    int floorZ = (int)floor(_p.z());
    double xDiff = _p.x() - floorX;
    double yDiff = _p.y() - floorY;
    double zDiff = _p.z() - floorZ;
    int xIdx = (floorX+1000) % 10;
    int yIdx = (floorY+1000) % 10;
    int zIdx = (floorZ+1000) % 10;

    //cout << "Start: " << Vector3D(xidx, yidx, zidx) << endl;
    // relative position in cube
    Vector3D relP(xIdx + xDiff, yIdx + yDiff, zIdx + zDiff); 
    //cout << "relP: " << relP << endl;

    for(int x=xIdx;x<=xIdx+1;++x)
    for(int y=yIdx;y<=yIdx+1;++y)
    for(int z=zIdx;z<=zIdx+1;++z)
    {
        Vector3D _relP(relP);
        int _x = x, _y = y, _z = z;
        if(x == 10)
        {
            _x = 0;
            _relP.setX(_relP.x()-10);
        }
        if(y == 10)
        {
            _y = 0;
            _relP.setY(_relP.y()-10);
        }
        if(z == 10)
        {
            _z = 0;
            _relP.setZ(_relP.z()-10);
        }
        srand(_x + 10*_y + 100*_z);
        Vector3D gradient = m_gradients[_x + 10*_y + 100*_z];
        gradient.normalize();
        // Let the gradient vector contribute proprotional to the distance]
        // from relP
        double contribution = max(0.0, 1.0-Vector3D(_x,_y,_z).distanceFrom(_relP));
        //cout << "gradient from " << Vector3D(_x, _y, _z);
        //cout << " C: " << contribution << endl;
        dispV = dispV + gradient*contribution;
    }

    dispV = dispV*scale;
    return Vector3D(v + dispV).normalized();
}
