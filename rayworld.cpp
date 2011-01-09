#include <iostream>
#include <pthread.h>

#include "rayworld.h"
#include "raysphere.h"
#include "rayobject.h"
#include "rayplane.h"
#include "math.h"
#include "raycolor.h"
#include "raymaterial.h"
#include "raytriangle.h"
using namespace std;

#define SMALL_NUMBER 0.01

struct ThreadTraceData
{
    int xStart;
    int xEnd;
    int yStart;
    int yEnd;
    Vector3D origin;
    int reflectionDepth;
    RayWorld* world;
    RayCanvas* canvas;
};

void* pixelTrace(void* threadData)
{
    ThreadTraceData* d = (ThreadTraceData*) threadData;
    for(int x=d->xStart;x<d->xEnd;++x)
    {
        for(int y=d->yStart;y<d->yEnd;++y)
        {
            // "Fire of" a ray through point x, y and collect hits
            std::vector<Vector3D> rays;
            Vector3D ray = d->canvas->vectorThrough(x, y);
            RayColor pixelColor = d->world->rayTrace(d->origin, 
                                                     ray, 
                                                     d->reflectionDepth);
            d->canvas->setColor(x, y, pixelColor);
        }
    }
    delete d;
}

RayWorld::RayWorld()
{    
    m_canvas = 0;
}

void RayWorld::addObject(RayObject* object)
{
    m_objects.push_back(object);
}

void RayWorld::addLightSource(PointLight* light) 
{
    m_lights.push_back(light);
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
    m_canvas = new RayCanvas(m_camera.location(), 
                             m_camera.lookat(), 
                             m_camera.up(), 
                             pixelWidth, 
                             pixelHeight);

    // Divide width between threads, the last thread gets the remaindor
    const int THREAD_COUNT = 2;
    pthread_t threads[THREAD_COUNT];
    int  widthPart = pixelWidth / THREAD_COUNT;
    int leftOvers = pixelWidth % THREAD_COUNT;
    int startWidth = 0;
    int endWidth = startWidth + widthPart;

    for(int i=0;i<THREAD_COUNT;++i)
    {        
        // Note: Thread is responsible for cleaning up this
        ThreadTraceData* threadData = new ThreadTraceData;
        threadData->canvas = m_canvas;
        threadData->xStart = startWidth;
        threadData->xEnd = endWidth;
        threadData->yStart = 0;
        threadData->yEnd = pixelHeight;
        threadData->origin = m_camera.location();
        threadData->reflectionDepth = 3;
        threadData->world = this;
        pthread_create(&threads[i], 0, pixelTrace, (void*) threadData);
        
        startWidth += widthPart;
        endWidth += widthPart;

        if(i == THREAD_COUNT - 2)
            endWidth += leftOvers;
    }

    for(int i=0;i<THREAD_COUNT;++i)
    {
        void* status;
        pthread_join(threads[i], &status);
    }

}

RayColor RayWorld::rayTrace(const Vector3D& start, 
                            const Vector3D& direction, 
                            int depth)
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
    RayObject* hitObject = intersection.object();
    RayMaterial hitMaterial = hitObject->material();

    // Loop all light sources, accumulate diffuse and specular
    float diffuse = 0;
    float specular = 0;

    std::vector<PointLight*>::iterator lightIt = m_lights.begin();
    std::vector<PointLight*>::iterator lightEnd = m_lights.end();
    while(lightIt != lightEnd)
    {
        Vector3D lightRay = (*lightIt)->position() - intersection.point();
        lightRay.normalize();
        // Calculate diffuse ligtning by dotting 
        // normal of intersection point with ray from light source
        float currDiffuse = lightRay.dotProduct(intersection.normal());
        // Stay positive!
        currDiffuse = max(currDiffuse, 0.f);

        int shadow = 1; // Shadow coefficent, 0 indicating point is in shadow
        // Calculate distance from intersction point to light source
        float disPointToLight = Vector3D(intersection.point() - 
                                (*lightIt)->position()).lengthSquared();

        // Loop over all objects in scene to see if 
        // anything is blocking the ray from light
        std::vector<RayObject*>::iterator objIt = m_objects.begin();
        std::vector<RayObject*>::iterator objEnd = m_objects.end();

        while(objIt != objEnd)
        {
            if(*objIt == intersection.object())
            {
                ++objIt;
                continue;
            }

            std::vector<Intersection> lightIntersections;
            if((*objIt)->intersects(intersection.point(), 
                                    lightRay, 
                                    lightIntersections))
            {
                for(int i=0;i<lightIntersections.size();++i)
                {
                    float distance = Vector3D(lightIntersections[i].point() - 
                                     (*lightIt)->position()).lengthSquared();
                    if(distance < disPointToLight)
                    {                        
                        shadow = 0;
                    }
                }                
            }
            ++objIt;
        }

        // Sum diffuse shading from all light sources
        diffuse = min(diffuse + currDiffuse*shadow, 1.f);
        // Add specular lightning
        Vector3D reflectionVector = direction - 
                                    intersection.normal()*
                                    direction.dotProduct(intersection.normal())*2;
        reflectionVector.normalize();

        float specularShading = reflectionVector.dotProduct(lightRay);
        if(specularShading<0)
            specularShading = 0;
        specularShading = pow(specularShading, hitMaterial.specPower());
        specular = min(specularShading, 1.0f);
        ++lightIt;
    }


    // Calculate reflection
    Vector3D reflectionVector = direction - 
                                intersection.normal()*
                                intersection.normal().dotProduct(direction)*2;
    reflectionVector.normalize();
    RayColor reflectionColor = rayTrace(intersection.point() +
                                        (reflectionVector*SMALL_NUMBER), 
                                        reflectionVector, 
                                        depth - 1);
    float reflection = hitMaterial.reflection();

    // Calculate refraction( not quite working ! )
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
            Vector3D refractionVector = (direction*n) + 
                                        refractionNormal*(n*cosIn - sqrt(cosOut));
            refractionVector.normalize();
            refractionColor = rayTrace(intersection.point() + 
                              refractionVector*0.01, refractionVector, depth - 1);
        }

    }
    // This is the lightning model    
    diffuse *= intersection.object()->material().diffuse();    
    specular *= intersection.object()->material().specular();

    RayColor pixColor;
    pixColor = hitColor.scaled(hitMaterial.ambient());
    pixColor += hitColor.scaled(diffuse).scaled(1-reflection);
    pixColor += reflectionColor.scaled(reflection);
    pixColor += RayColor(255,255,255).scaled(specular);
    return pixColor;
}

bool RayWorld::closestIntersection(const Vector3D& start, const Vector3D& direction, Intersection& closest)
{
    // Loop over all objects
    std::vector<RayObject*>::iterator objIt = m_objects.begin();
    std::vector<RayObject*>::iterator objEnd = m_objects.end();
    std::vector<Intersection> intersections;
    while(objIt != objEnd)
    {
        std::vector<Intersection> localIntersections;
        if((*objIt)->intersects(start, direction, localIntersections))
        {
            while(localIntersections.size())
            {
                intersections.push_back(*localIntersections.begin());
                localIntersections.erase(localIntersections.begin());
            }

        }
        ++objIt;
    }
    if(intersections.size() == 0)
    {
        return false;
    }

    // Find the closest intersection
    closest = *intersections.begin();
    intersections.erase(intersections.begin());
    float closestDistance = Vector3D(start - closest.point()).lengthSquared();

    std::vector<Intersection>::iterator it = intersections.begin();
    std::vector<Intersection>::iterator end = intersections.end();
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
