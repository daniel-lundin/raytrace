#ifndef PARSER_H
#define PARSER_H
#include <fstream>
#include <vector>
#include <list>
#include <utility>

#include "parseentity.h"

using std::istream;
using std::vector;

// foward declarations
class RayWorld;
class RayMaterial;
class Logger;



class Parser : public ParseEntity
{
public:
    Parser(RayWorld* world, Logger* logger);

    bool parse(istream& stream);
    void evaluateEntity(ParseEntity* entity);

    // ParseEntity overrides
    void addEntity(ParseEntity* entity);
    ParseEntity* parent();
    void dump();
private:
    void evaluateSphereEntity(ParseEntity* entity);
    void evaluatePlaneEntity(ParseEntity* entity);
    void evaluateLightEntity(ParseEntity* entity);
    RayMaterial evaluateMateriaEntity(ParseEntity* entity);
    void evaluateCameraEntity(ParseEntity* entity);
    // members
    RayWorld* m_world;
    Logger* m_logger;
    vector<ParseEntity*> m_entities;
    ParseEntity* m_current;
};

#endif // PARSER_H
