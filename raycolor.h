#ifndef RAYCOLOR_H
#define RAYCOLOR_H

#include <string>
using std::string;

class RayColor
{
public:
    RayColor();
    RayColor(float r, float g, float b);

    void setR(float r);
    void setG(float g);
    void setB(float b);

    float r() const;
    float g() const;
    float b() const;

    void scale(float factor);
    RayColor scaled(float scale);
    // Operators
    RayColor operator+(const RayColor&);
    RayColor& operator+=(const RayColor&);    

    std::string toString();
private:
    float m_r;
    float m_g;
    float m_b;

};

#endif // RAYCOLOR_H
