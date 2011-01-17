#include "rotation.h"
#include <math.h>
#include <string.h> // for memset

Rotation::Rotation(RayObject* obj, float xrot, float yrot, float zrot)
    : m_obj(obj), m_xrot(xrot), m_yrot(yrot), m_zrot(zrot)
{
}

bool Rotation::intersects(const Vector3D& start, 
                          const Vector3D& direction, 
                          std::vector<Intersection>&)
{
    float xrotMatrix[3][3];
    memset(xrotMatrix, 0, 3*3*sizeof(float));
    // Setup rotation matrix around x-axis
    xrotMatrix[0][0] = 1;
    xrotMatrix[1][1] = xrotMatrix[2][2] = cos(xrot);
    xrotMatrix[1][2] = - sin(xrot);
    xrotMatrix[2][1] = sin(xrot);

    // For rotation matrices X_transpose == X_inverse
    float xrotMatrixInv[3][3];
    memset(xrotMatrixInv, 0, 3*3*sizeof(float));
    for (i = 0; i < 3; ++i) 
    {
        for (j = 0; j < 3; ++j) 
        {
            xrotMatrixInv[i][j] = xrotMatrix[j][i];     
        }
    }

    
    return false;
}
