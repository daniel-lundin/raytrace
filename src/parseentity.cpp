#include "parseentity.h"
#include <string>
#include <iostream>
#include <sstream>
#include <stdexcept>

using std::cout;
using std::endl;

void throwParseError(int lineno, const std::string& str = "no info")
{
    std::ostringstream oss;
    oss << "Syntax error at line " << lineno << ": " << str;
    throw std::runtime_error(oss.str());
}

ParseEntity::ParseEntity(ParseEntity* _parent, const std::string& _type)
    : parent(_parent)
{
    if(_type == "sphere")
        type = SPHERE;
    else if(_type == "cylinder")
        type = CYLINDER;
    else if(_type == "camera")    
        type = CAMERA;
    else if(_type == "material")
        type = MATERIAL;
    else if(_type == "plane")
        type = PLANE;
    else if(_type == "light")
        type = LIGHT;
    else if(_type == "box")
        type = BOX;
    else if(_type == "difference")
        type = DIFFERENCE;
    else if(_type == "checker")
        type = CHECKER;
    else if(_type == "normal")
        type = NORMAL_MODIFIER;
}


void ParseEntity::addLine(const string& line, const int lineno)
{
    // Lines must be in one of two forms:
    // identifier: a b c
    // identifier: a
    size_t colonPos = line.find(':');
    if(colonPos == std::string::npos)
    {
        throwParseError(lineno, "Expected :");
    }
    std::istringstream iss(line);
    string identifier;
    if((iss >> identifier).fail())
        throwParseError(lineno, "expected identifier");
    identifier.replace(identifier.length()-1, 1, "");
    double x,y,z;
    if((iss >> x).fail())
    {
        throwParseError(lineno, "unable to parse float");
    }
    if(iss.eof())
    {
        valueTypes[identifier] = x;
        return;
    }
    if((iss >> y).fail())
    {
        cout << "Identifier: " << identifier << endl;
        throwParseError(lineno, "unable to parse float");
    }
    if((iss >> z).fail())
    {
        throwParseError(lineno, "unable to parse float");
    }
    coordTypes[identifier] = Vector3D(x,y,z);
}

void ParseEntity::addEntity(ParseEntity* entity)
{
    children[entity->type] = entity;
}
