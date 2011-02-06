#include <iostream>
#include <list>
#include <vector>
#include <utility>
#include "difference.h"
#include "vector3d.h"
#include "intersection.h"
#include "raysphere.h"
#include "raybox.h"
#include "translation.h"

using std::list;
using std::vector;
using std::cout;
using std::endl;
using std::pair;
using std::make_pair;
using std::string;

bool sphere_intersection()
{
    RaySphere s(Vector3D(0,0,0), 1, RayMaterial());
    Vector3D start(0,0,-2);
    Vector3D dir(0,0,1);
    vector<Intersection> isecs;
    return s.intersects(start, dir, isecs); 
}

bool box_intersection()
{
    return true;
}

bool difference_test()
{

    RaySphere s(Vector3D(0,0,0), 1, RayMaterial());
    RayBox b(1.1, .1, 1.1, RayMaterial());
    Difference d(&s, &Translation(&b, 0,0,0));

    Vector3D start(0,0,-2);
    Vector3D dir(0,0,1);
    vector<Intersection> isecs;
    d.intersects(start, dir, isecs); 
    cout << "Isecs size: " << isecs.size() << endl;
    return isecs.size() == 4;
}

int main(int argc, char** argv)
{
    list<pair<bool(*)(), string> > tests;
    tests.push_back(make_pair(&sphere_intersection, "Sphere intersection"));
    tests.push_back(make_pair(&box_intersection, "Box intersection"));
    tests.push_back(make_pair(&difference_test, "Difference intersection"));


    list<pair<bool(*)(), string> >::iterator it = tests.begin();
    list<pair<bool(*)(), string> >::iterator end = tests.end();

    unsigned int passes = 0;
    for(;it!=end;++it)
    {
        cout << "Running test: " << (*it).second << "...";
        if(it->first())
        {
            ++passes;
            cout << " PASSED!" << endl;
        }
        else
        {
            cout << " FAILED!" << endl;
        }
    }

    cout << passes << " out of " << tests.size() << " tests passed " << endl;
    
    return 0;
}
