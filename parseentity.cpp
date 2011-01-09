#include <string>
#include "parseentity.h"
#include <iostream>

using std::cout;
using std::endl;

ParseEntity::ParseEntity(const string& type, ParseEntity* parent)
    : m_parent(parent)
{
    if(type == "sphere")
        m_type = SPHERE;
    else if(type == "camera")    
        m_type = CAMERA;
    else if(type == "material")
        m_type = MATERIAL;
    else if(type == "plane")
        m_type = PLANE;
    else if(type == "light")
        m_type = LIGHT;
    
}

void ParseEntity::addLine(const string& line, const int lineno)
{
    m_lines.push_back(make_pair(lineno,line));
}

ParseEntities ParseEntity::type()
{
    return m_type;
}

void ParseEntity::addEntity(ParseEntity* entity)
{
    m_children.push_back(entity);
}

ParseEntity* ParseEntity::parent()
{
    return m_parent;
}

list<ParseEntity*>& ParseEntity::children()
{
    return m_children;
}
list<pair<int, string> >& ParseEntity::lines() 
{
    return m_lines;
}

void ParseEntity::dump()
{
    cout << "Entity: " << m_type << endl;
    
    list<pair<int, string> >::iterator it = m_lines.begin();
    list<pair<int, string> >::iterator end = m_lines.end();
    while(it != end)
    {
        cout << it->second << endl;
        ++it;
    }
    
    
    list<ParseEntity*>::iterator cit = m_children.begin();
    list<ParseEntity*>::iterator cend = m_children.end();
    while(cit != cend)
    {
        (*cit)->dump();
        ++cit;
    }
}
