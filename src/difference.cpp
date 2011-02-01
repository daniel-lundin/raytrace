#include <iostream>
#include <iterator>
#include <algorithm>
#include "difference.h"
#include "utils.h"

using std::copy;
using std::cout;
using std::endl;
using std::back_inserter;

Difference::Difference(RayObject* first, RayObject* second)
    : m_first(first), m_second(second)
{

}

        
bool Difference::intersects(const Vector3D& start, 
                            const Vector3D& direction,  
                            std::vector<Intersection>& isecs)
{
    std::vector<Intersection> firstIsecs;
    std::vector<Intersection> secondIsecs;
    // First check for intersections for the first object
    if(!m_first->intersects(start, direction, firstIsecs))
        return false;
    
    // Check for intersection on the other object, if it doesn't exist
    // Return the intersections for the first one only
    if(!m_second->intersects(start, direction, secondIsecs))
    {
        std::copy(firstIsecs.begin(), firstIsecs.end(), back_inserter(isecs));
        return true;
    }
    // Gather all intersection in one vector and sort them by distance
    // from point start
    std::vector<Intersection> allISecs;
    copy(firstIsecs.begin(), firstIsecs.end(), back_inserter(allISecs));
    copy(secondIsecs.begin(), secondIsecs.end(), back_inserter(allISecs));
    DistanceSorter sorter(start);
    std::sort(allISecs.begin(), allISecs.end(), sorter); 

    // Iterate through all intersection, while keeping track
    // of whether we're inside any of them
    bool insideFirst = false;
    bool insideSecond = false;
    std::vector<Intersection>::iterator it = allISecs.begin();
    std::vector<Intersection>::iterator end = allISecs.end();
    for(;it != end;++it)
    {
        if(it->object() == m_first)
        {
            // Hit on first object, add intersection only if not inside second
            if(!insideSecond)
            {
                isecs.push_back(*it);
            }
            insideFirst = !insideFirst;
        }
        else if(it->object() == m_second)
        {
            if(insideFirst && insideSecond)
            {
                // We're inside the first one 
                // and no longer inside the second(counting this hit)
                // so we have an intersection
                Intersection i(*it);
                i.setObject(m_first);
                i.setNormal(i.normal()*-1);
                isecs.push_back(i);
            }
            insideSecond = !insideSecond;
        }
    }
    return isecs.size() > 0;
}

