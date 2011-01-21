#ifndef VECTOR3D_H
#define VECTOR3D_H
#include <iostream>

class Vector3D
{
public:
    Vector3D();
    Vector3D(float x, float y, float z);
    // Compiler generated copy-/copy assignment constructors
    // works fine
     
    // Getters and setters
    float x() const;
    float y() const;
    float z() const;

    Vector3D& setX(float x);
    Vector3D& setY(float y);
    Vector3D& setZ(float z);

    // Operator overloads
    const Vector3D operator*(float scale) const;
    const Vector3D operator+(const Vector3D& v2) const;
    const Vector3D operator-(const Vector3D& v2) const;

    // Operations
    float length() const;
    float lengthSquared() const;

    float dotProduct(const Vector3D& other) const;
    Vector3D crossProduct(const Vector3D& other) const;

    Vector3D& normalize();
    Vector3D normalized() const;


    friend std::ostream& operator<<(std::ostream& os, const Vector3D&);
private:
    float m_x;
    float m_y;
    float m_z;
};

std::ostream& operator<<(std::ostream& os, const Vector3D&);
#endif // VECTOR3D_H
