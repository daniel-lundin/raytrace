#include "rayworld.h"
#include "raysphere.h"
#include "rayobject.h"
#include "rayplane.h"
#include "math.h"
#include "raycolor.h"
#include "raymaterial.h"
#include "raytriangle.h"
#include <iostream>
#include <QThreadPool>
#include <QDebug>
using namespace std;


PixelTracer::PixelTracer(int xStart,
                         int xEnd,
                         int yStart,
                         int yEnd,
                         const Vector3D& origin,
                         int reflectionDepth,
                         RayWorld* world,
                         RayCanvas* canvas)
    :   m_xStart(xStart),
        m_xEnd(xEnd),
        m_yStart(yStart),
        m_yEnd(yEnd),
        m_world(world),
        m_canvas(canvas),
        m_origin(origin),        
        m_reflectionDepth(reflectionDepth)
{
}

void PixelTracer::run()
{
    for(int x=m_xStart;x<m_xEnd;++x)
    {
        for(int y=m_yStart;y<m_yEnd;++y)
        {
            // "Fire of" a ray through point x, y and collect hits
            QVector<Vector3D> rays;
            Vector3D ray = m_canvas->vectorThrough(x, y);

            RayColor pixelColor = m_world->rayTrace(m_origin, ray, m_reflectionDepth);
            m_canvas->setColor(x, y, pixelColor);
        }
    }

}

RayWorld::RayWorld()
{    
    RayCamera camera;
    camera.setLocation(Vector3D(0,6,0));
    camera.setLookat(Vector3D(0,0,10));
    camera.setUp(Vector3D(0,1,0));
    m_camera = camera;

    m_objects.push_back(new RayPlane(Vector3D(0,0,0), Vector3D(0,1,0), RayMaterial(0.2, RayColor(60,40,40))));
    m_objects.push_back(new RayPlane(Vector3D(0,0,21), Vector3D(0,0,-1), RayMaterial(0.1, RayColor(140,140,76))));

   // m_objects.push_back(new RayTriangle(Vector3D(1,1,10), Vector3D(-1,1,10), Vector3D(0,1,10), RayMaterial(0, RayColor(255,0,255))));
    m_objects.push_back(new RaySphere(Vector3D(-1,2,15), 1.5, RayMaterial(0.2, RayColor(80,80,60), 0.7, 0.6)));
    m_objects.push_back(new RaySphere(Vector3D(2,2,9), 1.5, RayMaterial(0.2, RayColor(60,80,80), 0.7, 0.6)));

    m_lights.push_back(new PointLight(Vector3D(10,8,10)));
    //m_lights.push_back(new PointLight(Vector3D(-30,10,10)));

    m_canvas = 0;
}

void RayWorld::addObject(RayObject* object)
{
    m_objects.push_back(object);
}

void RayWorld::clearScene()
{
    m_objects.clear();
}

void RayWorld::setCamera(const RayCamera& camera)
{
    m_camera = camera;
}

void RayWorld::render(int pixelWidth, int pixelHeight)
{
    delete m_canvas;
    //Vector3D origin(0, 15, -2);
    //m_canvas = new RayCanvas(origin, Vector3D(0,0,10), Vector3D(0,1,0), pixelWidth, pixelHeight);
    m_canvas = new RayCanvas(m_camera.location(), m_camera.lookat(), m_camera.up(), pixelWidth, pixelHeight);

    // Divide width between threads, the last thread gets the remaindor
    const int THREAD_COUNT = 2;
    QList<QThread*> threads;
    int  widthPart = pixelWidth / THREAD_COUNT;
    int leftOvers = pixelWidth % THREAD_COUNT;
    int startWidth = 0;
    int endWidth = startWidth + widthPart;

    for(int i=0;i<THREAD_COUNT;++i)
    {        
        PixelTracer* traceThread = new PixelTracer(startWidth, endWidth, 0, pixelHeight, m_camera.location(), 3, this, m_canvas);
        threads.push_back(traceThread);
        traceThread->start();
        startWidth += widthPart;
        endWidth += widthPart;

        if(i == THREAD_COUNT - 2)
            endWidth += leftOvers;
    }

    for(int i=0;i<THREAD_COUNT;++i)
    {
        threads[i]->wait(10000);
    }

}

RayColor RayWorld::rayTrace(const Vector3D& start, const Vector3D& direction, int depth)
{
    if(depth < 0)
        return RayColor(0,0,0);
    // Find closest intersection
    Intersection intersection;
    if(!closestIntersection(start, direction, intersection))
    {
        return RayColor(0,0,0);
    }
    RayColor hitColor = intersection.color();
    float diffuse = 0;
    float specular = 0;

    // Loop all light sources
    QList<PointLight*>::iterator lightIt = m_lights.begin();
    QList<PointLight*>::iterator lightEnd = m_lights.end();    
    while(lightIt != lightEnd)
    {

        Vector3D lightRay = (*lightIt)->position() - intersection.point();
        lightRay.normalize();
        float currDiffuse = lightRay.dotProduct(intersection.normal());
        if(currDiffuse<0)
        {
            currDiffuse = 0;
        }        
        int shadow = 1; // Shadow coefficent, 0 indicating point is in shadow
        float disPointToLight = Vector3D(intersection.point() - (*lightIt)->position()).lengthSquared();

        QList<RayObject*>::iterator objIt = m_objects.begin();
        QList<RayObject*>::iterator objEnd = m_objects.end();

        while(objIt != objEnd)
        {
            if(*objIt == intersection.object())
            {
                ++objIt;
                continue;
            }

            QList<Intersection> lightIntersections;
            if((*objIt)->intersects(intersection.point(), lightRay, lightIntersections))
            {
                for(int i=0;i<lightIntersections.count();++i)
                {
                    float distance = Vector3D(lightIntersections[i].point() - (*lightIt)->position()).lengthSquared();
                    if(distance < disPointToLight)
                    {                        
                        shadow = 0;
                    }
                }                
            }
            ++objIt;
        }
        // Sum diffuse shading
        diffuse = min(diffuse + currDiffuse*shadow, 1.f);

        // Add specular lightning
        Vector3D reflectionVector = direction - intersection.normal()*direction.dotProduct(intersection.normal())*2;
        reflectionVector.normalize();

        float specularShading = reflectionVector.dotProduct(lightRay);
        if(specularShading<0)
            specularShading = 0;
        specularShading = pow(specularShading, 22);
        specular = min(specularShading, 1.0f);
        ++lightIt;
    }


    // Calculate reflection
    Vector3D reflectionVector = direction - intersection.normal()*intersection.normal().dotProduct(direction)*2;
    reflectionVector.normalize();
    RayColor reflectionColor = rayTrace(intersection.point()+(reflectionVector*0.01), reflectionVector, depth - 1);
    float reflection = intersection.object()->material().reflectionRate();

    // Calculate refraction
    RayColor refractionColor(0,0,0);
    if(intersection.object()->material().refractionRate() > 0)
    {
        Vector3D refractionNormal = intersection.normal();

        float n = intersection.object()->material().brytningsIndex();
        float cosIn = - refractionNormal.dotProduct(direction);
        float cosOut = 1.0f - n*n*(1.0f - cosIn*cosIn);
        if(cosOut > 0)
        {

        }
        float c1 = -direction.dotProduct(refractionNormal);
        float c2 = 1 - n*n*(1-c1*c1);
        if(c2 > 0)
        {
            Vector3D refractionVector = (direction*n) + refractionNormal*(n*cosIn - sqrt(cosOut));
            refractionVector.normalize();
            refractionColor = rayTrace(intersection.point() + refractionVector*0.01, refractionVector, depth - 1);
        }

    }
    //refractionVector.

    // This is the lightning model    
    //diffuse*= diffuse;
    diffuse = max(0.6f, diffuse);
    float refractionRate = intersection.object()->material().refractionRate();
    //diffuse = 1;
    RayColor pixColor;
    pixColor = hitColor.scaled(diffuse).scaled(1-reflection);
    pixColor += reflectionColor.scaled(reflection);
    pixColor += RayColor(255,255,255).scaled(specular);
    //pixColor = pixColor.scaled(1 - refractionRate) + refractionColor.scaled(refractionRate);
    return pixColor;
}

bool RayWorld::closestIntersection(const Vector3D& start, const Vector3D& direction, Intersection& closest)
{
    // Loop over all objects
    QList<RayObject*>::iterator objIt = m_objects.begin();
    QList<RayObject*>::iterator objEnd = m_objects.end();
    QList<Intersection> intersections;
    while(objIt != objEnd)
    {
        QList<Intersection> localIntersections;        
        if((*objIt)->intersects(start, direction, localIntersections))
            intersections.append(localIntersections);
        ++objIt;
    }
    if(intersections.size() == 0)
    {
        return false;
    }

    // Find the closest intersection
    closest = *intersections.begin();
    float closestDistance = Vector3D(start - closest.point()).lengthSquared();
    intersections.pop_front();
    QList<Intersection>::iterator it = intersections.begin();
    QList<Intersection>::iterator end = intersections.end();
    while(it != end)
    {
        float currentDistance = (start - it->point()).lengthSquared();
        if(currentDistance < closestDistance)
        {
            closest = *it;
            closestDistance = currentDistance;
        }
        ++it;
    }
    return true;
}

RayCanvas* RayWorld::canvas()
{
    return m_canvas;
}
