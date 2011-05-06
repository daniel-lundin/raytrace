#ifndef RANDOMNORMALDISPLACER_H
#define RANDOMNORMALDISPLACER_H

#include "rayobject.h"

class Perlin;

class RandomNormalDisplacer : public RayObject
{
public:
    RandomNormalDisplacer(RayObject* obj, double amount);
    ~RandomNormalDisplacer();
    bool intersects(const Vector3D& start, 
                    const Vector3D& direction, 
                    std::vector<Intersection>&);

    enum RandomType { BASIC };
private:
    RayObject* m_object;
    Perlin* m_perlin;
    double m_amount;
};
#endif
