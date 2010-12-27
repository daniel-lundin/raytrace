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

class ParseEntity
{
public:
    ParseEntity(const string& type, ParseEntity* parent);
    void addLine(const string& line, const int lineno);
    
    const string& type();
    virtual void addEntity(ParseEntity* entity);
    virtual ParseEntity* parent();
    virtual void dump();
private:
    string m_type;
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
    RayWorld* m_world;
    vector<ParseEntity*> m_entities;
    ParseEntity* m_current;
};

#endif // PARSER_H
