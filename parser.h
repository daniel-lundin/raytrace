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

enum ParseEntities {SPHERE, CAMERA, PLANE};

class ParseEntity
{
public:
    ParseEntity(const string& type, ParseEntity* parent);
    void addLine(const string& line, const int lineno);
    
    ParseEntities type();
    virtual void addEntity(ParseEntity* entity);
    virtual ParseEntity* parent();
    virtual void dump();
private:
    ParseEntities m_type;
    ParseEntity* m_parent;    
    list<pair<int, string> > m_lines;    
    list<ParseEntity*> m_children;
};

class Parser : public ParseEntity
{
public:
    Parser(RayWorld* world);

    bool parse(istream& stream);
    void evaluateEntity(ParseEntity* entity);

    // ParseEntity overrides
    void addEntity(ParseEntity* entity);
    ParseEntity* parent();
    void dump();
private:
    void evaluateSphereEntity(ParseEntity* entity);
    void evaluateCameraEntity(ParseEntity* entity);
    // members
    RayWorld* m_world;
    vector<ParseEntity*> m_entities;
    ParseEntity* m_current;
};

#endif // PARSER_H
