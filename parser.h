#ifndef PARSER_H
#define PARSER_H
#include <fstream>
#include <vector>
#include <list>
#include <utility>

using std::list;
using std::string;
using std::pair;
using std::istream;
using std::vector;
using std::pair;

// foward declarations
class RayWorld;
class RayMaterial;
class Logger;

enum ParseEntities {SPHERE, CAMERA, PLANE, MATERIAL};

class ParseEntity
{
public:
    ParseEntity(const string& type, ParseEntity* parent);
    void addLine(const string& line, const int lineno);
    
    ParseEntities type();
    virtual void addEntity(ParseEntity* entity);
    virtual ParseEntity* parent();
    list<ParseEntity*>& children();
    virtual void dump();
    list<pair<int, string> >& lines(); 
private:
    ParseEntities m_type;
    ParseEntity* m_parent;    
    list<pair<int, string> > m_lines;    
    list<ParseEntity*> m_children;
};

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
    RayMaterial evaluateMateriaEntity(ParseEntity* entity);
    void evaluateCameraEntity(ParseEntity* entity);
    // members
    RayWorld* m_world;
    Logger* m_logger;
    vector<ParseEntity*> m_entities;
    ParseEntity* m_current;
};

#endif // PARSER_H
