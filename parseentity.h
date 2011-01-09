#ifndef PARSEENTITY_H
#define PARSEENTITY_H

#include <list>

using std::list;
using std::string;
using std::pair;

enum ParseEntities {SPHERE, CAMERA, PLANE, MATERIAL, LIGHT};

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

#endif
