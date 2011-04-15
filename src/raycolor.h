#ifndef RAYCOLOR_H
#define RAYCOLOR_H

#include <string>
using std::string;

class RayColor
{
public:
    RayColor();
    RayColor(double r, double g, double b);

    void setR(double r);
    void setG(double g);
    void setB(double b);

    double r() const;
    double g() const;
    double b() const;

    void scale(double factor);
    RayColor scaled(double scale) const;
    void clamp();
    RayColor clamped() const;
    // Operators
    RayColor operator+(const RayColor&);
    RayColor& operator+=(const RayColor&);    

    std::string toString() const;
private:
    double m_r;
    double m_g;
    double m_b;

};

#endif // RAYCOLOR_H
