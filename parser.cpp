#include "parser.h"
#include "rayworld.h"
#include <iostream>
#include <sstream>

using std::vector;
using std::string;
using std::cout;
using std::endl;
using std::exception;
using std::istringstream;
using std::make_pair;

ParseEntity::ParseEntity(const string& type, ParseEntity* parent)
    : m_parent(parent)
{
    if(type == "sphere")
        m_type = SPHERE;
    else if(type == "camera")    
        m_type = CAMERA;
    
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

Parser::Parser(RayWorld* world)
    : m_world(world), ParseEntity("Parser", this)
{

}

bool Parser::parse(istream& stream)
{
    m_current = this;
    int lineno = 0;
    string line;
    while(getline(stream, line))
    {
        if(line.size() > 0 && line[0] != '#')
        {
            istringstream iss(line);
            string token;
            iss >> token;
	        if (token[0] == '.')
            {
                string entityType = token.substr(1, -1);    			
                ParseEntity* ent = new ParseEntity(entityType, m_current);
                m_current->addEntity(ent);
                m_current = ent;
	        }
            else if(token == "next")
            {
                m_current = m_current->parent();
            }
            else
            {
                m_current->addLine(line, lineno);
            }
        }
        ++lineno;
    }
    
    size_t entCount = m_entities.size();
    for(size_t i = 0; i < entCount; ++i)
    {
        ParseEntity* e = m_entities[i];
        evaluateEntity(e);
    }
    return true;
}

void Parser::evaluateEntity(ParseEntity* entity)
{
    switch(entity->type())
    {
        case SPHERE:
            evaluateSphereEntity(entity);
            break;
        case CAMERA:
            evaluateCameraEntity(entity);
            break;
        default:

    }
}

void Parser::addEntity(ParseEntity* entity)
{
    m_entities.push_back(entity);
}

ParseEntity* Parser::parent()
{
    throw exception();
}

void Parser::dump()
{
    vector<ParseEntity*>::iterator it = m_entities.begin();
    vector<ParseEntity*>::iterator end = m_entities.end();
    while(it != end)
    {
        (*it)->dump();
        ++it;
    }
}

void Parser::evaluateSphereEntity(ParseEntity* entity)
{
    e
}

void Parser::evaluateCameraEntity(ParseEntity* entity)
{
    
}
