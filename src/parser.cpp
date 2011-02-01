#include <iostream>
#include <sstream>
#include <stdexcept>

#include "parser.h"
#include "rayworld.h"
#include "raysphere.h"
#include "raycylinder.h"
#include "raybox.h"
#include "translation.h"
#include "rotation.h"
#include "loginterface.h"
#include "raycamera.h"

using std::vector;
using std::string;
using std::cout;
using std::endl;
using std::exception;
using std::runtime_error;
using std::istringstream;
using std::ostringstream;
using std::make_pair;

void throwParseError(int lineno, const std::string& str = "no info")
{
    ostringstream oss;
    oss << "Syntax error at line " << lineno << ": " << str;
    throw runtime_error(oss.str());
}

Parser::Parser(RayWorld* world, Logger* logger)
    : ParseEntity("Parser", this), m_world(world), m_logger(logger)
{

}

bool Parser::parse(istream& stream)
{
    m_logger->debug("Starting parse sequence");
    m_current = this;
    int lineno = 1;
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
        case CYLINDER:
            evaluateCylinderEntity(entity);
            break;
        case CAMERA:
            evaluateCameraEntity(entity);
            break;
        case BOX:
            evaluateBoxEntity(entity);
            break;
        case PLANE:
            evaluatePlaneEntity(entity);
            break;
        case LIGHT:
            evaluateLightEntity(entity);
            break;
        default:
            break;
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
    Vector3D pos;
    float radius; 

    list<pair<int, string> >::iterator it = entity->lines().begin();    
    list<pair<int, string> >::iterator end = entity->lines().end();    

    while (it != end)
    {
        istringstream iss(it->second);
        string token;
        iss >> token;
        if(token == "pos:")
        {
            float x, y, z;
            if((iss >> x).fail())
                throwParseError(it->first);

            if((iss >> y).fail())
                throwParseError(it->first);

            if((iss >> z).fail())
                throwParseError(it->first);
            
            pos = Vector3D(x, y, z);
           
        }
        else if (token == "radius:") 
        {
            if((iss >> radius).fail())
                throwParseError(it->first);
        }
        ++it;
    }
    // For now sphere can only have one children and that is material
    // Make sure that is the case
    RayMaterial m;
    list<ParseEntity*>& children = entity->children();

    if(children.size() == 1)
    {
        
        ParseEntity* material = *children.begin();
        if(material->type() != MATERIAL)
            throw runtime_error("Sphere has unknown child.");
        m = evaluateMateriaEntity(material);
    }

    
    // Note: this method should probably return the sphere instead
    // in case transformations, intersections etc should ever be implemented
    ostringstream oss;
    oss << "Adding sphere at " << pos.x() << ", " <<pos.y() << ", " << pos.z();
    oss << "Radius: " << radius;
    m_logger->info(oss.str());

    RaySphere* sphere = new RaySphere(pos, radius, m);
    m_world->addObject(sphere); 
}

void Parser::evaluateCylinderEntity(ParseEntity* entity)
{
    Vector3D pos;
    float xrot = 0;
	float yrot = 0;
	float zrot = 0;
    float radius = 0;
	float length = 0;

    list<pair<int, string> >::iterator it = entity->lines().begin();    
    list<pair<int, string> >::iterator end = entity->lines().end();    

    for(;it!=end;++it)
    {

        istringstream iss(it->second);
        string token;
        iss >> token;
        if(token == "pos:")
        {
            float x, y, z;
            if((iss >> x).fail())
                throwParseError(it->first);

            if((iss >> y).fail())
                throwParseError(it->first);

            if((iss >> z).fail())
                throwParseError(it->first);
            
            pos = Vector3D(x, y, z);
           
        }
        else if(token == "rot:")
        {
            float x, y, z;
            if((iss >> x).fail())
                throwParseError(it->first);

            if((iss >> y).fail())
                throwParseError(it->first);

            if((iss >> z).fail())
                throwParseError(it->first);
            xrot = x;
            yrot = y;
            zrot = z;
        } 
        else if(token == "radius:")
        {
            if((iss >> radius).fail())
                throwParseError(it->first);
        }
        else if(token == "length:")
        {
            if((iss >> length).fail())
                throwParseError(it->first);
        }
    }

    RayMaterial m;
    list<ParseEntity*>& children = entity->children();

    if(children.size() == 1)
    {
        ParseEntity* material = *children.begin();
        if(material->type() != MATERIAL)
            throw runtime_error("Sphere has unknown child.");
        m = evaluateMateriaEntity(material);
    }

    RayObject* o = new RayCylinder(radius, length, m);
    o = new Rotation(o, xrot, yrot, zrot);
    o = new Translation(o, pos.x(), pos.y(), pos.z());
    m_world->addObject(o);
}

void Parser::evaluateBoxEntity(ParseEntity* entity)
{
    Vector3D pos;
	Vector3D rot;
    float xsize = 0;
	float ysize = 0;
	float zsize = 0;

    list<pair<int, string> >::iterator it = entity->lines().begin();    
    list<pair<int, string> >::iterator end = entity->lines().end();    

    while (it != end)
    {
        istringstream iss(it->second);
        string token;
        
        iss >> token;
        float x,y,z;

        if((iss >> x).fail())
            throwParseError(it->first);

        if((iss >> y).fail())
            throwParseError(it->first);

        if((iss >> z).fail())
            throwParseError(it->first);
        if(token == "size:")
        {
            xsize = x;
            ysize = y;
            zsize = z;
        }
        else if(token == "pos:")
        {
            pos = Vector3D(x,y,z);
        }
		else if(token == "rot:")
		{
			rot = Vector3D(x, y, z);
		}
        ++it;
    }

    RayMaterial m;
    list<ParseEntity*>& children = entity->children();

    if(children.size() == 1)
    {
        ParseEntity* material = *children.begin();
        if(material->type() != MATERIAL)
            throw runtime_error("Sphere has unknown child.");
        m = evaluateMateriaEntity(material);
    }
    RayObject* o = new RayBox(xsize, ysize, zsize, m);
	o = new Rotation(o, rot.x(), rot.y(), rot.z());
    o = new Translation(o, pos.x(), pos.y(), pos.z());
    m_world->addObject(o);
}
void Parser::evaluatePlaneEntity(ParseEntity* entity)
{
    Vector3D pos, normal;

    list<pair<int, string> >::iterator it = entity->lines().begin();    
    list<pair<int, string> >::iterator end = entity->lines().end();    

    while (it != end)
    {
        istringstream iss(it->second);
        string token;

        iss >> token;

        float x, y, z;
        if((iss >> x).fail())
            throwParseError(it->first);

        if((iss >> y).fail())
            throwParseError(it->first);

        if((iss >> z).fail())
            throwParseError(it->first);
        if(token == "pos:")
            pos = Vector3D(x,y,z);
        else if(token == "normal:")
            normal = Vector3D(x,y,z);
        else
            throwParseError(it->first); 
        ++it;
    }

    // For now sphere can only have one children and that is material
    // Make sure that is the case
    list<ParseEntity*>& children = entity->children();
    if(children.size() != 1)
        throw runtime_error("Sphere missing material definition");
    ParseEntity* material = *children.begin();
    if(material->type() != MATERIAL)
        throw runtime_error("Sphere missing material definition");

    RayMaterial m = evaluateMateriaEntity(material);

    m_world->addObject(new RayPlane(pos, normal, m));
}


void Parser::evaluateLightEntity(ParseEntity* entity)
{
    Vector3D position;

    list<pair<int, string> >::iterator it = entity->lines().begin();    
    list<pair<int, string> >::iterator end = entity->lines().end();    
    while(it != end)
    {
        istringstream iss(it->second);
        string token;
        if((iss >> token).fail())
            throwParseError(it->first);
        if(token == "pos:")
        {
            float x,y,z;
             
            if((iss >> x).fail())
                throwParseError(it->first);

            if((iss >> y).fail())
                throwParseError(it->first);

            if((iss >> z).fail())
                throwParseError(it->first);
            position = Vector3D(x,y,z);
        }
        else
        {
            throwParseError(it->first);
        }
        ++it;
    }
    m_world->addLightSource(new PointLight(position));
}

RayMaterial Parser::evaluateMateriaEntity(ParseEntity* entity)
{
    float ambient 		= 0;
	float diffuse 		= 0;
	float specular 		= 0;
	float specpower 	= 0;
	float reflection 	= 0;
	float refraction    = 0;
	float refractionIndex = 0;
    int r = 0;
	int g = 0;
	int b = 0;
    list<pair<int, string> >::iterator it = entity->lines().begin();    
    list<pair<int, string> >::iterator end = entity->lines().end();    
    while(it != end)
    {
        istringstream iss(it->second);
        string token;
        float value;
        if((iss >> token).fail())
            throwParseError(it->first);

        if((iss >> value).fail())
            throwParseError(it->first);

        if(token == "ambient:")
            ambient = value;
        else if(token == "diffuse:") 
            diffuse = value;
        else if(token == "specular:")
            specular = value;
        else if(token == "specpower:")
            specpower = value;
        else if(token == "reflection:")
            reflection = value;
        else if(token == "refraction:")
            refraction = value;
        else if(token == "refractionindex:")
            refractionIndex = value;
        else if(token == "color:")
        {
            r = value;
            if((iss >> g).fail())
                throwParseError(it->first);
            if((iss >> b).fail())
                throwParseError(it->first);
        }
        else
            throwParseError(it->first);

        ++it;
    }
    ostringstream oss;
    oss << "Material: " << RayColor(r,g,b).toString();
    oss << "Amb: " << ambient << " Dif: " << diffuse;
    oss << "Spec: " << specular << " specPower: " << specpower;
    oss << "Reflection: " << reflection;
	oss << "RefractionL: " << refraction;
	oss << "RefractionIndex: " << refractionIndex;
    m_logger->debug(oss.str());

    return RayMaterial(RayColor(r,g,b), 
                       ambient, 
                       diffuse, 
                       specular, 
                       specpower,
                       reflection,
					   refraction,
					   refractionIndex);
                
}

void Parser::evaluateCameraEntity(ParseEntity* entity)
{
    m_logger->info("Parsing camera");
    Vector3D location, lookat, up;
    bool locSet, lookSet, upSet;
    locSet = lookSet = upSet = false;
    
    list<pair<int, string> >::iterator it = entity->lines().begin();    
    list<pair<int, string> >::iterator end = entity->lines().end();    
    while(it != end)
    {
        string token;
        float x,y,z;
        istringstream iss(it->second);
        iss >> token;

        if((iss >> x).fail())
            throwParseError(it->first);

        if((iss >> y).fail())
            throwParseError(it->first);

        if((iss >> z).fail())
            throwParseError(it->first);
            
        Vector3D v(x, y, z);
        if(token == "location:")
        {
            location = v;
            locSet = true;
        }
        else if(token == "lookat:")
        {
            lookat = v;
            lookSet = true;
        }
        else if(token == "up:")
        {
            up = v;
            upSet = true;
        }
        else
            throwParseError(it->first);

        ++it;
    }
    if(!(locSet && lookSet && upSet))
       throwParseError(-1, "Missing attribute for camera definition"); 
    m_world->setCamera(RayCamera(location, lookat, up));
}
