#ifndef RAYWORLD_H
#define RAYWORLD_H
#include <vector>

#include "raycanvas.h"
#include "raycamera.h"

// Forward declarations
class Progress;
class PointLight;
class RayObject;
class PhotonIntersection;
class KDTreeNode;
class RayColor;

class RayWorld
{
public:
    RayWorld(Progress*);    
    void render(int pixelWidth, int pixelHeight);


    RayColor rayTrace(const Vector3D& start, 
                      const Vector3D& direction, 
                      int currDepth,
                      int maxDepth);

    void addObject(RayObject*);
    void addLightSource(PointLight*);
    void clearScene();

    void setCamera(const RayCamera& camera);
    RayCanvas* canvas();
    bool closestIntersection(const Vector3D& start, const Vector3D& direction, Intersection&);
    void buildPhotonMap();
    KDTreeNode* m_KDRoot;
    PhotonIntersection* nearestNeighbour(const Vector3D& point);
    void nearestNeighbours(const Vector3D& point, double r, std::vector<PhotonIntersection*>& neighbours);
private:
    void photonTrace(const Vector3D& start, 
                     const Vector3D& dir, 
                     const RayColor& color,
                     int depth=3);
    Progress* m_progress;
    RayCamera m_camera;
    std::vector<RayObject*> m_objects;
    std::vector<PointLight*> m_lights;
    std::vector<PhotonIntersection*> m_photonIsecs;
    RayCanvas* m_canvas;


};

#endif // RAYWORLD_H
