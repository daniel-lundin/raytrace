#include "rotation.h"
#include "matrix.h"
#include <string.h> // for memset

Rotation::Rotation(RayObject* obj, float xrot, float yrot, float zrot)
    : m_obj(obj), m_xrot(xrot), m_yrot(yrot), m_zrot(zrot)
{
}

bool Rotation::intersects(const Vector3D& start, 
                          const Vector3D& direction, 
                          std::vector<Intersection>& isecs)
{
    MATRIX3D xrot, yrot, zrot;
    generateXRot(xrot, m_xrot);
    generateYRot(yrot, m_yrot);
    generateZRot(zrot, m_zrot);

    MATRIX3D totrot,temprot;
    apply(xrot, yrot, temprot);
    apply(temprot, zrot, totrot);
    // For rotation matrices X_transpose == X_inverse
    MATRIX3D totrotInv;
    transpose(totrot, totrotInv);

    // Instead of transforming the object, transform the ray with the 
    // inverse transform
    Vector3D start_i = apply(totrotInv, start);
    Vector3D dir_i = apply(totrotInv, direction);

    if(!m_obj->intersects(start_i, dir_i, isecs))
        return false;

    std::vector<Intersection>::iterator it = isecs.begin();
    std::vector<Intersection>::iterator end = isecs.end();
    for (; it != end; ++it)
    {
        it->setObject(this);
        // transform intersection point
        it->setPoint(apply(totrot, it->point()));
        
        // transform intersection normal
        it->setNormal(apply(totrot, it->normal()));

    }

    
    return true;
}
