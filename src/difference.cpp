#include <iostream>
#include <iterator>
#include <algorithm>
#include "difference.h"
#include "utils.h"

using std::copy;
using std::cout;
using std::endl;
using std::back_inserter;
using std::for_each;
using std::mem_fun;
using std::bind2nd;
using std::vector;

Difference::Difference(RayObject* first, RayObject* second)
    : m_first(first), m_second(second)
{

}

        
bool Difference::intersects(const Vector3D& start, 
                            const Vector3D& direction,  
                            std::vector<Intersection>& isecs)
{
    vector<Intersection> firstIsecs;
    vector<Intersection> secondIsecs;
    // First check for intersections for the first object
    if(!m_first->intersects(start, direction, firstIsecs))
        return false;
   

 
    // Check for intersection on the other object, if it doesn't exist
    // Return the intersections for the first one only
    if(!m_second->intersects(start, direction, secondIsecs))
    {
        vector<Intersection>::iterator it = firstIsecs.begin(); 
        vector<Intersection>::iterator end = firstIsecs.end(); 
        while(it != end)
        {
            it->setObject(this);
            ++it;
        }
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
    //cout << "All isecs count : " << allISecs.size() << endl;
    RayMaterial lastMaterial;
    RayObject* lastHit = 0;
    for(;it != end;++it)
    {
        if(it->object() == m_first)
        {
            //cout << "Hit on first" << endl;
            // Hit on first object, add intersection only if not inside second
            lastMaterial = it->material();
            if(!insideSecond)
            {
                //cout << "\tAdding hit on first obj" << endl;
                lastHit = m_first;
                it->setObject(this);
                it->setInsideHit(false);
                isecs.push_back(*it);
            }
            insideFirst = !insideFirst;
        }
        else if(it->object() == m_second)
        {
            //cout << "Hit on second" << endl;
            if(insideFirst)
            {
                //cout << "\tAdding hit on second obj" << endl;
                // We're inside the first one 
                // and no longer inside the second(counting this hit)
                // so we have an intersection
                lastHit = m_second;
                it->setNormal(it->normal()*-1.0);
                it->setMaterial(lastMaterial);
                it->setObject(this);
                it->setInsideHit(true);
                isecs.push_back(*it);
            }
            insideSecond = !insideSecond;
        }
    }
    return isecs.size() > 0;
}

