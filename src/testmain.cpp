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
#include "photonintersection.h"
#include "kdtree.h"

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
    return isecs.size() == 4;
}

bool kdtree_test()
{
    bool success = true;
    std::vector<PhotonIntersection*> isecs;
    PhotonIntersection* i1 = new PhotonIntersection(Vector3D(0,4,-3), Vector3D());
    isecs.push_back(i1);
    PhotonIntersection* i2 = new PhotonIntersection(Vector3D(1,2,6), Vector3D());
    isecs.push_back(i2);
    PhotonIntersection* i3 = new PhotonIntersection(Vector3D(2,3,0), Vector3D());
    isecs.push_back(i3);
    PhotonIntersection* i4 = new PhotonIntersection(Vector3D(4,-2,0), Vector3D());
    isecs.push_back(i4);
    PhotonIntersection* i5 = new PhotonIntersection(Vector3D(3,3,1), Vector3D());
    isecs.push_back(i5);
    PhotonIntersection* i6 = new PhotonIntersection(Vector3D(6,3,-1), Vector3D());
    isecs.push_back(i6);
    PhotonIntersection* i7 = new PhotonIntersection(Vector3D(0,3,0), Vector3D());
    isecs.push_back(i7);
    KDTreeNode* root = KDTreeNode::makeTree(isecs);
    KDTreeNode* nearest = root->NNSearch(Vector3D(0, 2, 0));
    success &= nearest->position().lengthSquared() == i7->position.lengthSquared();
    nearest = root->NNSearch(Vector3D(0, 4, -2));
    success &= nearest->position().lengthSquared() == i1->position.lengthSquared();


    return success;
}

int main(int argc, char** argv)
{
    list<pair<bool(*)(), string> > tests;
    tests.push_back(make_pair(&sphere_intersection, "Sphere intersection"));
    tests.push_back(make_pair(&box_intersection, "Box intersection"));
    tests.push_back(make_pair(&difference_test, "Difference intersection"));
    tests.push_back(make_pair(&kdtree_test, "KD-tree test"));


    list<pair<bool(*)(), string> >::iterator it = tests.begin();
    list<pair<bool(*)(), string> >::iterator end = tests.end();

    unsigned int passes = 0;
    cout << "-- Running tests --" << endl;
    for(;it!=end;++it)
    {
        cout << "test: " << (*it).second << "...";
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

    cout << "Result: " << passes << " out of " << tests.size() << " tests passed " << endl;
    
    return 0;
}
