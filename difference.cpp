#include "difference.h"
#include <iterator>
#include <algorithm>

using std::copy;
using std::back_inserter;

// Function object for sorting intersections based on the distance from some point p
struct distanceSorter
{
    distanceSorter(const Vector3D& p) : m_p(p) {}
    bool operator()(const Intersection& first, const Intersection& second)
    {
        float d1 = Vector3D(first.point() - m_p).lengthSquared();
        float d2 = Vector3D(second.point() - m_p).lengthSquared();
        return d1 < d2;
    }
    const Vector3D& m_p;
};
bool isCloser(const Intersection& first, const Intersection& second)
{

}
Difference::Difference(RayObject* first, RayObject* second)
    : m_first(first), m_second(second)
{

}

        
bool Difference::intersects(const Vector3D& start, 
                            const Vector3D& direction,  
                            std::vector<Intersection>& isecs)
{
    std::vector<Intersection> firstIsecs;
    // First check for intersections for the first object
    if(m_first->intersects(start, direction, firstIsecs))
    {
        std::vector<Intersection> secondIsecs;
        // Check for intersection on the other object, if it doesn't exist
        // Return the intersections for the first one only
        if(!m_second->intersects(start, direction, secondIsecs))
        {
            std::copy(firstIsecs.begin(), firstIsecs.end(), back_inserter(isecs));
            return true;
        }
        // The ray intersects both objects
        // Sort the intersections by distance from start.
        std::vector<Intersection> allISecs;
        copy(firstIsecs.begin(), firstIsecs.end(), back_inserter(allISecs));
        copy(secondIsecs.begin(), secondIsecs.end(), back_inserter(allISecs));

                                 
    } 
    return false;
}

