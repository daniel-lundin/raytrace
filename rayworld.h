#ifndef RAYWORLD_H
#define RAYWORLD_H
#include "rayobject.h"
#include <list>
#include "raycanvas.h"
#include "raycamera.h"
#include "pointlight.h"
#include <QList>
#include <QRunnable>
#include <QThread>
using std::list;

class RayWorld;

class PixelTracer : public QThread
{
public:
    PixelTracer(int xStart,
                int xEnd,
                int yStart,
                int yEnd,                
                const Vector3D& origin,
                int reflectionDepth,
                RayWorld* world,
                RayCanvas* canvas);
    void run();

private:
    int m_xStart;
    int m_xEnd;
    int m_yStart;
    int m_yEnd;
    Vector3D m_origin;
    int m_reflectionDepth;

    RayCanvas* m_canvas;
    RayWorld* m_world;
};

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
    QList<RayObject*> m_objects;
    QList<PointLight*> m_lights;
    RayCanvas* m_canvas;

};

#endif // RAYWORLD_H
