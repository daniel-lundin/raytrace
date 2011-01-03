#include "raytriangle.h"
#include "rayplane.h"

RayTriangle::RayTriangle(const Vector3D& v1, const Vector3D& v2, const Vector3D& v3, const RayMaterial& material)
    : m_v1(v1), m_v2(v2), m_v3(v3), m_material(material)
{
    Vector3D v12 = m_v2 - m_v1;
    Vector3D v13 = m_v3 - m_v1;
    Vector3D planeNormal = v12.crossProduct(v13);
    m_plane = new RayPlane(v1, planeNormal, RayMaterial(RayColor(), 1, 1, 1, 1, 1));
}

RayTriangle::~ RayTriangle()
{
    delete m_plane;
}



bool RayTriangle::intersects(const Vector3D& start, const Vector3D& direction, std::vector<Intersection>& intersections)
{
    std::vector<Intersection> planeIntersections;
    if(!m_plane->intersects(start, direction, planeIntersections))
        return false;
    Intersection i = *planeIntersections.begin();
    return false;
    // form to vector from sides on triangle
    Vector3D side1 = m_v2 - m_v1;
    Vector3D side2 = m_v3 - m_v1;
    // m_v1 + a * side1 + b * side2 = intersection point
    //a*side1.x + b*side.x = i.point().x() - m_v1.x() ;
    //a*side1.y + b*side.y = i.point().y() - m_v1.y();
                           // if 0 < a < 1, 0 < b < 1 and a+b < 1 hit is inside triangle


    // Create three planes from ray origin to each pair of triangle
    // Find the normals
    Vector3D n1 = (m_v1 - i.point()).crossProduct(m_v2 - start);
    Vector3D n2 = (m_v1 - start).crossProduct(m_v3 - start);
    Vector3D n3 = (m_v2 - start).crossProduct(m_v3 - start);
    if((i.point() - m_v1).dotProduct(n1) < 0)
        return false;
    if((i.point() - m_v1).dotProduct(n2) < 0)
        return false;
    if((i.point() - m_v2).dotProduct(n3) < 0)
        return false;
    i.setObject(this);
    i.setNormal(m_plane->m_up);
    i.setColor(RayColor(255,0,255));   
    intersections.push_back(i);
    return true;
}

RayMaterial RayTriangle::material()
{
    return m_material;
}
