#include <iostream>
#include <sstream>
#include <stdexcept>

#include "parser.h"
#include "pointlight.h"
#include "rayworld.h"
#include "raysphere.h"
#include "raycylinder.h"
#include "raybox.h"
#include "translation.h"
#include "rotation.h"
#include "loginterface.h"
#include "raycamera.h"
#include "difference.h"
#include "randomnormaldisplacer.h"
#include "checkertexture.h"

using std::vector;
using std::string;
using std::cout;
using std::endl;
using std::exception;
using std::runtime_error;
using std::istringstream;
using std::ostringstream;
using std::make_pair;

void throwParseDoubleError(int lineno)
{
    ostringstream oss;
    oss << "Error parsing double at line " << lineno;
    throw runtime_error(oss.str());
}

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
        if (e->type() == LIGHT) 
        {
            m_world->addLightSource(evaluateLightEntity(e));
        }
        else if(e->type() == CAMERA)
        {
            m_world->setCamera(evaluateCameraEntity(e));
        }
        else
        {
            m_world->addObject(evaluateEntity(e));
        }
    }
    return true;
}

RayObject* Parser::evaluateEntity(ParseEntity* entity)
{
    switch(entity->type())
    {
        case SPHERE:
            return evaluateSphereEntity(entity);
            break;
        case CYLINDER:
            return evaluateCylinderEntity(entity);
            break;
        case BOX:
            return evaluateBoxEntity(entity);
            break;
        case PLANE:
            return evaluatePlaneEntity(entity);
            break;
        case DIFFERENCE:
            return evaluateDifferenceEntity(entity);
            break;
        case TRANSLATION:
            return evaluateTranslationEntity(entity);
            break;
        default:
            throw std::exception();
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

RayObject* Parser::evaluateSphereEntity(ParseEntity* entity)
{
    Vector3D pos;
    double radius; 

    list<pair<int, string> >::iterator it = entity->lines().begin();    
    list<pair<int, string> >::iterator end = entity->lines().end();    

    while (it != end)
    {
        istringstream iss(it->second);
        string token;
        iss >> token;
        if(token == "pos:")
        {
            double x, y, z;
            if((iss >> x).fail())
                throwParseDoubleError(it->first);

            if((iss >> y).fail())
                throwParseDoubleError(it->first);

            if((iss >> z).fail())
                throwParseDoubleError(it->first);
            
            pos = Vector3D(x, y, z);
           
        }
        else if (token == "radius:") 
        {
            if((iss >> radius).fail())
                throwParseDoubleError(it->first);
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
    
    ostringstream oss;
    oss << "Adding sphere at " << pos.x() << ", " <<pos.y() << ", " << pos.z();
    oss << "Radius: " << radius;
    m_logger->info(oss.str());

    //return new RaySphere(pos, radius, m);
    return new RandomNormalDisplacer(new RaySphere(pos, radius, m), 0.06);
}

RayObject* Parser::evaluateCylinderEntity(ParseEntity* entity)
{
    Vector3D pos;
    double xrot = 0;
	double yrot = 0;
	double zrot = 0;
    double radius = 0;
	double length = 0;

    list<pair<int, string> >::iterator it = entity->lines().begin();    
    list<pair<int, string> >::iterator end = entity->lines().end();    

    for(;it!=end;++it)
    {

        istringstream iss(it->second);
        string token;
        iss >> token;
        if(token == "pos:")
        {
            double x, y, z;
            if((iss >> x).fail())
                throwParseDoubleError(it->first);

            if((iss >> y).fail())
                throwParseDoubleError(it->first);

            if((iss >> z).fail())
                throwParseDoubleError(it->first);
            
            pos = Vector3D(x, y, z);
           
        }
        else if(token == "rot:")
        {
            double x, y, z;
            if((iss >> x).fail())
                throwParseDoubleError(it->first);

            if((iss >> y).fail())
                throwParseDoubleError(it->first);

            if((iss >> z).fail())
                throwParseDoubleError(it->first);
            xrot = x;
            yrot = y;
            zrot = z;
        } 
        else if(token == "radius:")
        {
            if((iss >> radius).fail())
                throwParseDoubleError(it->first);
        }
        else if(token == "length:")
        {
            if((iss >> length).fail())
                throwParseDoubleError(it->first);
        }
    }

    RayMaterial m;
    list<ParseEntity*>& children = entity->children();

    if(children.size() == 1)
    {
        ParseEntity* material = *children.begin();
        if(material->type() != MATERIAL)
            throw runtime_error("Cylinder has unknown child.");
        m = evaluateMateriaEntity(material);
    }

    RayObject* o = new RayCylinder(radius, length, m);
    o = new Rotation(o, xrot, yrot, zrot);
    o = new Translation(o, pos.x(), pos.y(), pos.z());
    return o;
}

RayObject* Parser::evaluateBoxEntity(ParseEntity* entity)
{
    Vector3D pos;
	Vector3D rot;
    double xsize = 0;
	double ysize = 0;
	double zsize = 0;

    list<pair<int, string> >::iterator it = entity->lines().begin();    
    list<pair<int, string> >::iterator end = entity->lines().end();    

    while (it != end)
    {
        istringstream iss(it->second);
        string token;
        
        iss >> token;
        double x,y,z;

        if((iss >> x).fail())
            throwParseDoubleError(it->first);

        if((iss >> y).fail())
            throwParseDoubleError(it->first);

        if((iss >> z).fail())
            throwParseDoubleError(it->first);
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
    return o;
}

RayObject* Parser::evaluatePlaneEntity(ParseEntity* entity)
{
    Vector3D pos, normal;

    list<pair<int, string> >::iterator it = entity->lines().begin();    
    list<pair<int, string> >::iterator end = entity->lines().end();    

    while (it != end)
    {
        istringstream iss(it->second);
        string token;

        iss >> token;

        double x, y, z;
        if((iss >> x).fail())
            throwParseDoubleError(it->first);

        if((iss >> y).fail())
            throwParseDoubleError(it->first);

        if((iss >> z).fail())
            throwParseDoubleError(it->first);
        if(token == "pos:")
            pos = Vector3D(x,y,z);
        else if(token == "normal:")
            normal = Vector3D(x,y,z);
        else
            throwParseError(it->first, "Unknown attribute '" + token + "'"); 
        ++it;
    }

    // For now sphere can only have one children and that is material
    // Make sure that is the case
    list<ParseEntity*>& children = entity->children();
    if(children.size() != 1)
        throw runtime_error("Plane missing material definition");
    ParseEntity* material = *children.begin();
    if(material->type() != MATERIAL)
        throw runtime_error("Plane missing material definition");

    RayMaterial m = evaluateMateriaEntity(material);

    return new RandomNormalDisplacer(new RayPlane(pos, normal, m), 0.03);
    return new RayPlane(pos, normal, m);
}

RayObject* Parser::evaluateDifferenceEntity(ParseEntity* entity)
{
    list<ParseEntity*>& children = entity->children();
    
    if(children.size() < 2)
        throw std::runtime_error("difference must have at least two children");

    Difference* diff;
    list<ParseEntity*>::iterator it = children.begin();
    list<ParseEntity*>::iterator end = children.end();

    RayObject* r1 = evaluateEntity(*it);
    ++it;
    RayObject* r2 = evaluateEntity(*it);
    diff = new Difference(r1, r2);
    ++it;
    while(it != end)
    {
        diff = new Difference(diff, evaluateEntity(*it)); 
        ++it;
    }     
    return diff;
}

RayObject* Parser::evaluateTranslationEntity(ParseEntity* entity)
{
    list<pair<int, string> >::iterator it = entity->lines().begin();

    istringstream iss(it->second);
    string token;
    iss >> token;

    double x,y,z;

    if((iss >> x).fail())
        throwParseError(it->first);

    if((iss >> y).fail())
        throwParseError(it->first);

    if((iss >> z).fail())
        throwParseError(it->first);

    return new Translation(evaluateEntity(*entity->children().begin()), x, y, z);
}

PointLight* Parser::evaluateLightEntity(ParseEntity* entity)
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
            double x,y,z;
             
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
    return new PointLight(position);
}

RayMaterial Parser::evaluateMateriaEntity(ParseEntity* entity)
{
    double ambient 		= 0;
	double diffuse 		= 0;
	double specular 		= 0;
	double specpower 	= 0;
	double reflection 	= 0;
	double refraction    = 0;
	double refractionIndex = 0;
    int r = 0;
	int g = 0;
	int b = 0;
    list<pair<int, string> >::iterator it = entity->lines().begin();    
    list<pair<int, string> >::iterator end = entity->lines().end();    
    while(it != end)
    {
        istringstream iss(it->second);
        string token;
        double value;
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

RayCamera Parser::evaluateCameraEntity(ParseEntity* entity)
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
        double x,y,z;
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
    return RayCamera(RayCamera(location, lookat, up));
}
