#include "rotation.h"
#include <math.h>
#include <string.h> // for memset

Rotation::Rotation(RayObject* obj, float xrot, float yrot, float zrot)
    : m_obj(obj), m_xrot(xrot), m_yrot(yrot), m_zrot(zrot)
{
}

bool Rotation::intersects(const Vector3D& start, 
                          const Vector3D& direction, 
                          std::vector<Intersection>& isecs)
{
    float xrotMatrix[3][3];
    memset(xrotMatrix, 0, 3*3*sizeof(float));
    // Setup rotation matrix around x-axis
    xrotMatrix[0][0] = 1;
    xrotMatrix[1][1] = xrotMatrix[2][2] = cos(m_xrot);
    xrotMatrix[1][2] = - sin(m_xrot);
    xrotMatrix[2][1] = sin(m_xrot);

    // For rotation matrices X_transpose == X_inverse
    float xrotMatrixInv[3][3];
    memset(xrotMatrixInv, 0, 3*3*sizeof(float));
    for (int i = 0; i < 3; ++i) 
    {
        for (int j = 0; j < 3; ++j) 
        {
            xrotMatrixInv[i][j] = xrotMatrix[j][i];     
        }
    }

    // Instead of transformting the object we transform the ray with the 
    // inverse
    Vector3D start_i, dir_i;
    float vec[3];
    // Apply inverse transformation on start
    for (int i = 0; i <3; i++) 
    {
        vec[i] = (xrotMatrixInv[i][0]*start.x() +
                 xrotMatrixInv[i][1]*start.y() +
                 xrotMatrixInv[i][2]*start.z());
    }
    start_i = Vector3D(vec[0], vec[1], vec[2]);

    // Apply inverse transformation on direction
    for (int i = 0; i <3; i++) 
    {
        vec[i] = (xrotMatrixInv[i][0]*direction.x() +
                 xrotMatrixInv[i][1]*direction.y() +
                 xrotMatrixInv[i][2]*direction.z());
    }
    dir_i = Vector3D(vec[0], vec[1], vec[2]);

    if(!m_obj->intersects(start_i, dir_i, isecs))
        return false;

    std::vector<Intersection>::iterator it = isecs.begin();
    std::vector<Intersection>::iterator end = isecs.end();
    for (; it != end; ++it)
    {
        // Inverse transform intersection point
        for(int i=0;i<3;++i)
        {
            vec[i] = (xrotMatrix[i][0]*it->point().x() +
                     xrotMatrix[i][1]*it->point().y() +
                     xrotMatrix[i][2]*it->point().z());
        }
        it->setPoint(Vector3D(vec[0], vec[1], vec[2]));
        
        // Inverse transform intersection normal
        for(int i=0;i<3;++i)
        {
            vec[i] = (xrotMatrix[i][0]*it->normal().x() +
                     xrotMatrix[i][1]*it->normal().y() +
                     xrotMatrix[i][2]*it->normal().z());
        }
        it->setNormal(Vector3D(vec[0], vec[1], vec[2]));

    }

    
    return true;
}
