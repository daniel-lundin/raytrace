#ifndef PARSEENTITY_H
#define PARSEENTITY_H

#include <list>
#include <vector>
#include <map>
#include "vector3d.h"

using std::list;
using std::vector;
using std::string;
using std::pair;

// Forward declarations
class RayWorld;

enum EntityType {SPHERE, 
                    CYLINDER, 
                    PLANE, 
                    BOX, 
                    CAMERA,
                    MATERIAL, 
                    LIGHT, 
                    DIFFERENCE,
                    TRANSLATION};

struct ParseEntity
{
    ParseEntity(ParseEntity* parent, const string&);
    EntityType createEntity(const string& type);
    void addLine(const string& line, const int lineno);
    
    virtual void addEntity(ParseEntity* entity);

    std::map<string, Vector3D> coordTypes;
    std::map<string, double> valueTypes;

    std::map<EntityType, ParseEntity*> children;
    EntityType type;
    ParseEntity* parent;    
};

#endif
