#ifndef RAYWORLD_H
#define RAYWORLD_H
#include "rayobject.h"
#include <list>
#include "raycanvas.h"
#include "raycamera.h"
#include "pointlight.h"
#include <vector>

using std::list;

class RayWorld;


class RayWorld
{
public:
    RayWorld();    
    void render(int pixelWidth, int pixelHeight);

    void addObject(RayObject*);
    void clearScene();

    void setCamera(const RayCamera& camera);
    RayCanvas* canvas();

    RayColor rayTrace(const Vector3D& start, const Vector3D& direction, int depth);
    bool closestIntersection(const Vector3D& start, const Vector3D& direction, Intersection&);
private:
    RayCamera m_camera;
    std::vector<RayObject*> m_objects;
    std::vector<PointLight*> m_lights;
    RayCanvas* m_canvas;

};

#endif // RAYWORLD_H
