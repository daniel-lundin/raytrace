#include <iostream>
#include <algorithm>
#include <pthread.h>
#include <time.h> // to seed rand

#include "rayworld.h"
#include "photonintersection.h"
#include "pointlight.h"
#include "rayobject.h"
#include "rayplane.h"
#include "math.h"
#include "raycolor.h"
#include "raymaterial.h"
#include "raybox.h"
#include "utils.h"
#include "progress.h"
#include "kdtree.h"

using namespace std;

#define PI 3.14159265
#define SMALL_NUMBER 0.01
RayMaterial testMaterial1()
{
    return RayMaterial(RayColor(130,90,90), 0.2,0.8,0.3,30,0,0,0);
}
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
    Progress* progress;
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
            Vector3D canvasPoint = d->canvas->pointAt(x, y);
            Vector3D ray = d->canvas->vectorThrough(x, y);

#if 0
            std::vector<std::pair<Vector3D, Vector3D> > DOFRays;
            d->canvas->DOFVectors(x, y, DOFRays);
            RayColor color(0,0,0);
            for(unsigned int i=0;i<DOFRays.size();++i) {
                color += d->world->rayTrace(DOFRays[i].first, 
                                            DOFRays[i].second, 
                                            d->reflectionDepth);
            }

            color.scale(1.0/DOFRays.size());
            d->canvas->setColor(x, y, color);
#else
            RayColor pixelColor = d->world->rayTrace(d->origin, 
                                                     ray, 
                                                     0,
                                                     d->reflectionDepth);
            d->canvas->setColor(x, y, pixelColor);
#endif            
            
            d->progress->tick();
        }
    }
    delete d;
    return 0;
}

RayWorld::RayWorld(Progress* progress)
    : m_KDRoot(0),m_progress(progress)
{    
    m_canvas = 0;
}

void RayWorld::render(int pixelWidth, int pixelHeight)
{
    delete m_canvas;
    m_canvas = new RayCanvas(m_camera.location(), 
                             m_camera.lookat(), 
                             m_camera.up(), 
                             pixelWidth, 
                             pixelHeight,
                             18, 0.31, 8);

    //buildPhotonMap();

#ifdef NUM_THREADS
    // Divide width between threads, the last thread gets the remaindor
    pthread_t threads[NUM_THREADS];
    int  widthPart = pixelWidth / NUM_THREADS;
    int leftOvers = pixelWidth % NUM_THREADS;
    int startWidth = 0;
    int endWidth = startWidth + widthPart;

    for(int i=0;i<NUM_THREADS;++i)
    {        
        // Note: Thread is responsible for cleaning up this
        ThreadTraceData* threadData = new ThreadTraceData;
        threadData->canvas = m_canvas;
        threadData->xStart = startWidth;
        threadData->xEnd = endWidth;
        threadData->yStart = 0;
        threadData->yEnd = pixelHeight;
        threadData->origin = m_camera.location();
        threadData->reflectionDepth = 2;
        threadData->world = this;
        threadData->progress = m_progress;

        pthread_create(&threads[i], 0, pixelTrace, (void*) threadData);
        
        startWidth += widthPart;
        endWidth += widthPart;

        if(i == NUM_THREADS - 2)
            endWidth += leftOvers;
    }

    for(int i=0;i<NUM_THREADS;++i)
    {
        void* status;
        pthread_join(threads[i], &status);
    }
#else
    for(int i=0;i<pixelWidth;++i) for(int j=0;j<pixelHeight;++j)
    {
            Vector3D ray = m_canvas->vectorThrough(i, j);

            RayColor pixelColor = rayTrace(m_camera.location(),
                                                     ray, 
                                                     0,
                                                     2);
            m_canvas->setColor(i, j, pixelColor);
    }

#endif

}

RayColor RayWorld::rayTrace(const Vector3D& start, 
                            const Vector3D& direction, 
                            int currDepth,
                            int maxDepth)
{
    if(currDepth >= maxDepth)
        return RayColor(0, 0, 0);

    // Find closest intersection
    Intersection intersection;
    if(!closestIntersection(start, direction, intersection))
    {
        return RayColor(0,0,0);
    }
    RayColor hitColor = intersection.material().color();
    RayMaterial hitMaterial = intersection.material();

    // Loop all light sources, accumulate diffuse and specular
    double diffuse = 0;
    double specular = 0;

    std::vector<PointLight*>::iterator lightIt = m_lights.begin();
    std::vector<PointLight*>::iterator lightEnd = m_lights.end();
    for(;lightIt != lightEnd;++lightIt)
    {
        Vector3D lightRay =  (*lightIt)->position() - intersection.point();
        Vector3D lightToPoint = lightRay * -1;
        lightRay.normalize();
        // Shot another ray from light to intersection point
        // to see if point is in shadow

        Intersection lightISec;
        if(closestIntersection((*lightIt)->position(),
                               lightToPoint,
                               lightISec))
        {

            if(lightISec.object() != intersection.object())
                continue;
        }
        else
        {
            continue;
        }
        
        // Calculate diffuse ligtning by dotting 
        // normal of intersection point with ray from light source
        double currDiffuse = lightRay.dotProduct(intersection.normal());
        // Stay positive!
        currDiffuse = max(currDiffuse, 0.0);
        // Sum diffuse shading from all light sources
        diffuse = min(diffuse + currDiffuse, 1.0);
        // Add specular lightning
        Vector3D reflectionVector = mirror(direction, intersection.normal());
        reflectionVector.normalize();
        Vector3D halfway = direction*-1 + lightRay;
        halfway.normalize();


        //double specShading = exp(-pow(0.1*intersection.normal().dotProduct(halfway),2));
        double specShading = intersection.normal().dotProduct(halfway);
        //double specShading = reflectionVector.dotProduct(lightRay);
        if(specShading < 0)
            specShading = 0;
        specShading = pow(specShading, hitMaterial.specPower());
        specular = min(specShading, 1.0);
    }


    // Calculate reflection
    RayColor reflectionColor;
    double reflection = hitMaterial.reflection();
    if(reflection != 0)
    { 
        Vector3D reflectionVector = mirror(direction, intersection.normal());
        reflectionVector.normalize();
        // Do diffuse reflections only for first level
        // Hardly noticable in deeper recursions
        if(currDepth == -1)
        {
            // Find two perpendicular vectors to reflectionVector
            Vector3D notTheSame = reflectionVector;
            notTheSame.setX(notTheSame.x()+1);
            Vector3D v1 = reflectionVector.crossProduct(notTheSame);
            Vector3D v2 = reflectionVector.crossProduct(v1);
            v1.normalize();
            v2.normalize();

            int rays=64;
            for(int i=0; i<rays;++i)
            {
                Vector3D v = reflectionVector;
                // Walk in circle with increasing radius, didn't do it
                //v = v+v1*(sin((2*i*PI)/16)*(double)i/160.0);
                //v = v+v2*(cos((2*i*PI)/16)*(double)i/160.0);
                v = v+v1*((double)(rand()%10)/100);
                v = v+v2*((double)(rand()%10)/100);
                v.normalize();
                reflectionColor += rayTrace(intersection.point() +
                                            v*SMALL_NUMBER,
                                            v,
                                            currDepth+1,
                                            maxDepth);
            }
            reflectionColor = reflectionColor.scaled(1.0/rays);

        }
        else
        {
            
            reflectionColor = rayTrace(intersection.point() +
                                   (reflectionVector*SMALL_NUMBER), 
                                   reflectionVector, 
                                   currDepth + 1,
                                   maxDepth);
        }
    }

    // Calculate refraction vector
    RayColor refractionColor; 
    double refraction = hitMaterial.refraction();
    if(refraction != 0)
    {
        Vector3D normal = intersection.normal();
        double n1 = 1;
        double n2 = hitMaterial.refractionIndex();
        if(false && intersection.insideHit())
        {
            normal = normal*-1;
            std::swap(n1, n2);
        }
        Vector3D refractionVector = refract(direction, 
                                            normal,
                                            n1,
                                            n2);

        refractionColor = rayTrace(intersection.point() + 
                                   (refractionVector*SMALL_NUMBER),
                                   refractionVector,
                                   currDepth + 1,
                                   maxDepth);
    }
    // This is the lightning model    
    diffuse *= intersection.material().diffuse();    
    specular *= intersection.material().specular();

    RayColor pixColor;
    pixColor = hitColor.scaled(hitMaterial.ambient());
    pixColor += hitColor.scaled(diffuse);
    pixColor += reflectionColor.scaled(reflection);
    pixColor += RayColor(255.0,255.0,255.0).scaled(specular);
    pixColor = pixColor.scaled(1-refraction) + refractionColor.scaled(refraction);
    return pixColor;
    // Add color from photon map

    //pixColor = RayColor();
#if 1
    PhotonIntersection* photon = this->nearestNeighbour(intersection.point());
    if(Vector3D::distance(photon->position, intersection.point()) < 0.02)
    {
        double dot = intersection.normal().dotProduct(photon->incomingDirection*-1);
        pixColor = RayColor(255,255,255).scaled(dot);
    }
    //return RayColor();
#endif
#if 1
    double radius = 1;
    std::vector<PhotonIntersection*> photons;
    this->nearestNeighbours(intersection.point(), radius, photons);
    if(photons.size() == 0)
    {
        return pixColor;
        return RayColor(255,0,0);//pixColor;
    }

    RayColor photonColor;
    for(int i=0; i<photons.size();++i)
    {
        double photonAmount = intersection.normal().dotProduct(photons[i]->incomingDirection*-1);
        photonAmount *= 1 - Vector3D::distance(photons[i]->position, intersection.point())/radius;
        //photonAmount *= intersection.material().diffuse();
        photonAmount = max(0.0, photonAmount);
        if(photonAmount > 1.0)
            throw std::exception();
        photonColor += photons[i]->color.scaled(photonAmount);
    }
    pixColor += photonColor.scaled(1.0/photons.size());//(PI*pow(radius, 2)));
    //cout << Vector3D::distance(node->position(), intersection.point()) << endl;
    return pixColor;
#endif

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
        (*objIt)->intersects(start, direction, localIntersections);
        std::copy(localIntersections.begin(), 
                  localIntersections.end(), 
                  back_inserter(intersections));
        ++objIt;
    }
    if(intersections.size() == 0)
    {
        return false;
    }

    DistanceSorter distanceSorter(start);
    std::sort(intersections.begin(), intersections.end(), distanceSorter);
    closest = *intersections.begin();
    return true;
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
RayCanvas* RayWorld::canvas()
{
    return m_canvas;
}


PhotonIntersection* RayWorld::nearestNeighbour(const Vector3D& point)
{
    double closest = 99999999.0;
    PhotonIntersection* best = 0;
    int size = m_photonIsecs.size();
    for(int i=0;i<size;++i)
    {
        double dis = Vector3D::distanceSquared(m_photonIsecs[i]->position, point); 
        if(dis < closest)
        {
            closest = dis;
            best = m_photonIsecs[i];
        }
    }
    return best;
}

void RayWorld::nearestNeighbours(const Vector3D& point, 
                                 double r, 
                                 std::vector<PhotonIntersection*>& neighbours)
{
    
    int size = m_photonIsecs.size();
    for(int i=0;i<size;++i)
    {
        double dis = Vector3D::distance(m_photonIsecs[i]->position, point); 
        if(dis < r)
        {
            neighbours.push_back(m_photonIsecs[i]);
        }
    }
}

void RayWorld::photonTrace(const Vector3D& start, 
                           const Vector3D& dir,
                           const RayColor& color,
                           int depth)
{
    if(depth == 0)
        return;
    Intersection intersection;
    if(!closestIntersection(start, dir, intersection))
    {
        return;
    }
    // Todo: A real BDRF!

    double prob = (rand() % 1000) / 1000.0;
    double dotangle = dir.dotProduct(intersection.normal()*-1);

    //if(prob > dotangle)
    //    return;
    m_photonIsecs.push_back(new PhotonIntersection(intersection.point(), 
                                                   dir,
                                                   color));

    double vx = (rand() % 1000)/500.0 - 1;
    double vy = (rand() % 1000)/500.0 - 1;
    double vz = (rand() % 1000)/500.0 - 1;
    Vector3D reflection = mirror(dir, intersection.normal());//Vector3D(vx, vy, vz);//
    photonTrace(intersection.point() + reflection*SMALL_NUMBER, 
                reflection, 
                intersection.material().color().scaled(0.5),
                depth-1);

}

void RayWorld::buildPhotonMap()
{
    std::vector<PointLight*>::iterator it = m_lights.begin();
    std::vector<PointLight*>::iterator end = m_lights.end();
    while(it != end)
    {
        // Thousand photon
        int noPhotons = 1000;
        for(int i=0;i<noPhotons;++i)
        {
            double vx = (rand() % 1000)/500.0 - 1;
            double vy = (rand() % 1000)/500.0 - 1;
            double vz = (rand() % 1000)/500.0 - 1;
            Vector3D ray(vx,vy,vz);
            ray.normalize();
            double x = (rand() % 1000)/1000.0 - 0.5;
            double y = (rand() % 1000)/1000.0 - 0.5;
            double z = (rand() % 1000)/1000.0 - 0.5;
            Vector3D p((*it)->position().x() + x, 
                       (*it)->position().y() + y, 
                       (*it)->position().z() + z);
            photonTrace(p, ray, RayColor(255,255,255), 3);
        }
        ++it;
    }
    cout << "PhotonMap size: " << m_photonIsecs.size() << endl;
    //m_KDRoot = KDTreeNode::makeTree(m_photonIsecs);
}
