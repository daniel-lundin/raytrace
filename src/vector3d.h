#ifndef VECTOR3D_H
#define VECTOR3D_H
#include <iostream>

class Vector3D
{
public:
    Vector3D();
    Vector3D(double x, double y, double z);
    // Compiler generated copy-/copy assignment constructors
    // works fine
     
    // Getters and setters
    double x() const;
    double y() const;
    double z() const;

    Vector3D& setX(double x);
    Vector3D& setY(double y);
    Vector3D& setZ(double z);

    // Operator overloads
    const Vector3D operator*(double scale) const;
    const Vector3D operator+(const Vector3D& v2) const;
    const Vector3D operator-(const Vector3D& v2) const;

    // Operations
    double length() const;
    double lengthSquared() const;

    double dotProduct(const Vector3D& other) const;
    Vector3D crossProduct(const Vector3D& other) const;

    Vector3D& normalize();
    Vector3D normalized() const;


    friend std::ostream& operator<<(std::ostream& os, const Vector3D&);
private:
    double m_x;
    double m_y;
    double m_z;
};

std::ostream& operator<<(std::ostream& os, const Vector3D&);
#endif // VECTOR3D_H
