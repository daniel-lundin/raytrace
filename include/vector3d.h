#ifndef VECTOR3D_H
#define VECTOR3D_H
#include <iostream>

/** 
 * \brief Class representing a vector in 3D space
 *
 * This class can perform basic vector operations such
 * ass vector addition, subtraction, dot products, cross products,
 * normalizations.
 */
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
    /**
     * Vector scaling
     * @param scale
     */
    const Vector3D operator*(double scale) const;
    /**
     * Vector addition
     * @params v2 - other vector
     * @return Result of addition
     */
    const Vector3D operator+(const Vector3D& v2) const;
    const Vector3D operator-(const Vector3D& v2) const;

    // Operations
    /**
     * Calculates length of vector.
     *
     */
    double length() const;
    /**
     * Calculates square length of vector.
     * When comparing vector lengths, this is more efficient than
     * the length() method since it doesn't perform any square root operation.
     */
    double lengthSquared() const;
    double distanceFrom(const Vector3D& p) const;
    static double distance(const Vector3D& v1, const Vector3D& v2);
    static double distanceSquared(const Vector3D& v1, const Vector3D& v2);

    double dotProduct(const Vector3D& other) const;
    Vector3D crossProduct(const Vector3D& other) const;

    Vector3D& normalize();
    Vector3D normalized() const;
    Vector3D scaled(double factor) const;


    friend std::ostream& operator<<(std::ostream& os, const Vector3D&);
private:
    double m_x;
    double m_y;
    double m_z;
};

std::ostream& operator<<(std::ostream& os, const Vector3D&);
#endif // VECTOR3D_H
