#include "raycanvas.h"
#include "vector3d.h"
#include <iostream>

using namespace std;

RayCanvas::RayCanvas(const Vector3D& origin, const Vector3D& lookat, const Vector3D& up, int pixWidth, int pixHeight)
    : m_origin(origin), m_pixWidth(pixWidth), m_pixHeight(pixHeight)
{
    float d = 2.0;
    Vector3D direction = lookat - m_origin;
    direction.normalize();

    m_side = up.normalized().crossProduct(direction);
    m_up = direction.crossProduct(m_side);
    Vector3D canvasCenter = origin + direction*d;

    //m_focalPlane = RayPlane();

    m_upperLeft = canvasCenter + m_side + m_up;    
    m_pixelColors.resize(pixHeight*pixWidth);
}


Vector3D RayCanvas::vectorThrough(int pixelRow, int pixelCol)
{

    Vector3D canvasPoint = m_upperLeft - m_side*((float)pixelRow/m_pixWidth)*2 - m_up*((float)pixelCol/m_pixWidth)*2;

    Vector3D v = canvasPoint - m_origin;
    v.normalize();
    return v;
}

void RayCanvas::setColor(int x, int y, const RayColor& color)
{
    int index = y*m_pixWidth + x;
    if (index > m_pixelColors.size()-1)
    {
        return;
    }
    m_pixelColors[index] = color;
}

RayColor RayCanvas::color(int x, int y)
{
    int index = y*m_pixWidth + x;
    if (index > m_pixelColors.size()-1)
    {
        //qDebug() << "color out of range " << pixelRow << pixelCol;
        //qDebug() << m_pixelColors.size();
        return RayColor();
    }
    return m_pixelColors[index];
}
