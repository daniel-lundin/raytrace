#ifndef RAYCANVAS_H
#define RAYCANVAS_H
#include "vector3d.h"
#include <QVector>
#include "raycolor.h"
#include "rayplane.h"
class RayCanvas
{
public:
    RayCanvas(const Vector3D& origin, const Vector3D& lookat, const Vector3D& up, int pixWidth, int pixHeight);
    Vector3D vectorThrough(int pixelRow, int pixelCol);

    void setColor(int pixelRow, int pixelCol, const RayColor&);
    RayColor color(int pixelRow, int pixelCol);
private:   
    int m_pixWidth;
    int m_pixHeight;


    QVector<RayColor> m_pixelColors;

    Vector3D m_upperLeft;
    //RayPlane m_focalPlane;
    Vector3D m_origin;
    Vector3D m_side;
    Vector3D m_up;

};



#endif // RAYCANVAS_H
