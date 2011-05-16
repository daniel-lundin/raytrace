#ifndef PARSER_H
#define PARSER_H
#include <fstream>
#include <vector>
#include <list>
#include <utility>

#include "parseentity.h"
#include "raycamera.h"

using std::istream;
using std::vector;

// foward declarations
class RayWorld;
class RayMaterial;
class Logger;
class RayObject;
class PointLight;


class Parser : public ParseEntity
{
public:
    Parser(RayWorld* world, Logger* logger);

    bool parse(istream& stream);
    RayObject* evaluateEntity(ParseEntity* entity);

    // ParseEntity overrides
    void addEntity(ParseEntity* entity);
    ParseEntity* parent();
private:
    RayObject* evaluateDifferenceEntity(ParseEntity* entity);
    RayObject* evaluatePlaneEntity(ParseEntity* entity);
    RayObject* evaluateCylinderEntity(ParseEntity* entity);
    RayObject* evaluateBoxEntity(ParseEntity* entity);
    RayObject* evaluateSphereEntity(ParseEntity* entity);
    PointLight* evaluateLightEntity(ParseEntity* entity);
    RayCamera evaluateCameraEntity(ParseEntity* entity);
    RayMaterial evaluateMaterialEntity(ParseEntity* entity);

    // Utilities
    RayObject* parseRotation(RayObject*, ParseEntity*);
    RayObject* parseTranslation(RayObject*, ParseEntity*);
    RayObject* parseNormalModifier(RayObject*, ParseEntity*);
    RayObject* parseCheckerTexture(RayObject*, ParseEntity*);
    // members
    RayWorld* m_world;
    ParseEntity* m_current;
    Logger* m_logger;
    vector<ParseEntity*> m_entities;
};

#endif // PARSER_H
