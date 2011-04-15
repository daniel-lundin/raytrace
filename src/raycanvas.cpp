#include "raycanvas.h"
#include "vector3d.h"
#include "raymaterial.h"
#include "intersection.h"
#include <iostream>
#include <utility>

using namespace std;
#define AX 1.0
#define AY .8

RayCanvas::RayCanvas(const Vector3D& origin, 
                     const Vector3D& lookat, 
                     const Vector3D& up, 
                     int pixWidth, 
                     int pixHeight,
                     double focalLength,
                     double apertureSize,
                     int raysPerPixel)
    : m_origin(origin), 
      m_pixWidth(pixWidth), 
      m_pixHeight(pixHeight), 
      m_apertureSize(apertureSize),
      m_raysPerPixel(raysPerPixel)
{
    double d = 2;
    Vector3D direction = lookat - m_origin;
    direction.normalize();

    m_side = up.normalized().crossProduct(direction);
    m_up = direction.crossProduct(m_side);
    Vector3D canvasCenter = origin + direction*d;

    Vector3D focalCenter = origin + direction*focalLength;
    cout << "Focalcenter: " << focalCenter << endl;
    m_focalPlane = RayPlane(focalCenter, direction*-1, RayMaterial());

    m_upperLeft = canvasCenter + m_side*(AX/2) + m_up*(AY/2);    
    m_pixelColors.resize(pixHeight*pixWidth);
}


Vector3D RayCanvas::vectorThrough(int pixelRow, int pixelCol) const
{

    Vector3D canvasPoint = m_upperLeft - 
                           m_side*((double)pixelRow/m_pixWidth)*AX - 
                           m_up*((double)pixelCol/m_pixHeight)*AY;

    Vector3D v = pointAt(pixelRow, pixelCol) - m_origin;
    v.normalize();
    return v;
}

Vector3D RayCanvas::pointAt(int pixelRow, int pixelCol) const
{
    return m_upperLeft - 
           m_side*((double)pixelRow/m_pixWidth)*AX - 
           m_up*((double)pixelCol/m_pixHeight)*AY;
}

void RayCanvas::DOFVectors(int pixelRow, int pixelCol, std::vector<std::pair<Vector3D, Vector3D> >& rays)
{
    // find intersection with focal plane
    Vector3D v = this->vectorThrough(pixelRow, pixelCol);
    std::vector<Intersection> isecs;
    if(!m_focalPlane.intersects(m_origin, v, isecs))
        return;
    Vector3D focalPoint = isecs.begin()->point();
    //cout << pixelRow << ", " << pixelCol << " vector: " << v << " Focal point: " << focalPoint << endl;

    Vector3D canvasPoint = this->pointAt(pixelRow, pixelCol);
    Vector3D upleft = canvasPoint - 
                      m_up*m_apertureSize - 
                      m_side*m_apertureSize;
    std::vector<Vector3D> startPoints;
    for(int i=0;i<m_raysPerPixel;++i)
    {
        for(int j=0;j<m_raysPerPixel;++j)
        {
            Vector3D point = upleft + 
                            m_side*((double) i/m_raysPerPixel)*m_apertureSize +
                            m_up*((double) j/m_raysPerPixel)*m_apertureSize;
            startPoints.push_back(point);


        }
    }
    for(unsigned int i=0;i<startPoints.size();++i)
    {
        Vector3D direction = focalPoint - startPoints[i];
        direction.normalize();
        rays.push_back(make_pair<Vector3D, Vector3D>(startPoints[i], direction));
    }
}

void RayCanvas::setColor(unsigned int x, unsigned int y, const RayColor& color)
{
    unsigned int index = y*m_pixWidth + x;
    if (index > m_pixelColors.size()-1)
    {
        return;
    }

    m_pixelColors[index] = color.clamped();
}

RayColor RayCanvas::color(unsigned int x, unsigned int y) const
{
    unsigned int index = y*m_pixWidth + x;
    if (index > m_pixelColors.size()-1)
    {
        return RayColor();
    }
    return m_pixelColors[index];
}
