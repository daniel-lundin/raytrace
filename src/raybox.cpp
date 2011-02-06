#include "raybox.h"
#include "rayplane.h"
#include <vector>
#include <iostream>
#define EPS 0.01

RayBox::RayBox(float x, float y, float z, const RayMaterial& m)
    : m_x(x), m_y(y), m_z(z), m_material(m)
{}


bool RayBox::intersects(const Vector3D& start,
                        const Vector3D& end,
                        std::vector<Intersection>& isecs)
{
    // Check intersection with all six planes
    RayMaterial m;
    Vector3D normals[6];
    Vector3D points[6];
    
    normals[0] = points[0] = Vector3D(m_x, 0, 0);
    normals[1] = points[1] = Vector3D(-m_x, 0, 0);
    normals[2] = points[2] = Vector3D(0, m_y, 0);
    normals[3] = points[3] = Vector3D(0, -m_y, 0);
    normals[4] = points[4] = Vector3D(0, 0, m_z);
    normals[5] = points[5] = Vector3D(0, 0, -m_z);

    for (int i = 0; i < 6; ++i) 
    {
        RayPlane p(points[i], normals[i].normalized(), m);
        std::vector<Intersection> planeIsecs;
        if(!p.intersects(start, end, planeIsecs))
            continue;

        Intersection& isecPoint(*planeIsecs.begin());
        // Now check if point is inside the box
        if(isecPoint.point().x() + EPS > -m_x && 
           isecPoint.point().x() - EPS < m_x &&  
           isecPoint.point().y() + EPS > -m_y && 
           isecPoint.point().y() - EPS < m_y &&
           isecPoint.point().z() + EPS > -m_z &&
           isecPoint.point().z() - EPS < m_z)
        {
            isecPoint.setMaterial(m_material);
            isecPoint.setObject(this);
            bool close = false;
            //for (unsigned int i = 0; i < isecs.size(); i++) {
                //if(Vector3D(isecs[0].point() - isecPoint.point()).lengthSquared() < 0.1)
            //    {
             //       close = true;
              //      break;
               // }
           // }
            if(!close)
                isecs.push_back(isecPoint);
        }
    }
    return isecs.size();
}
