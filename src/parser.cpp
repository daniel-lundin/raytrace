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

Parser::Parser(RayWorld* world, Logger* logger)
    : ParseEntity(this, "Parser"), m_world(world), m_logger(logger)
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
                ParseEntity* ent = new ParseEntity(m_current, entityType);
                m_current->addEntity(ent);
                m_current = ent;
            }
            else if(token == "next")
            {
                m_current = m_current->parent;
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
        if (e->type == LIGHT) 
        {
            m_world->addLightSource(evaluateLightEntity(e));
        }
        else if(e->type == CAMERA)
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
    switch(entity->type)
    {
        case SPHERE:
            return evaluateSphereEntity(entity);
            break;
        case BOX:
            return evaluateBoxEntity(entity);
            break;
        case CYLINDER:
            return evaluateCylinderEntity(entity);
            break;
        case PLANE:
            return evaluatePlaneEntity(entity);
            break;
        case DIFFERENCE:
            return evaluateDifferenceEntity(entity);
            break;
        default:
            throw std::runtime_error("Unknown entity type");
            break;
    }
}

void Parser::addEntity(ParseEntity* entity)
{
    m_entities.push_back(entity);
}

ParseEntity* Parser::parent()
{
    throw std::runtime_error("Parser has not parent");
}

RayObject* Parser::evaluateSphereEntity(ParseEntity* entity)
{
    double radius; 
    std::map<string, double>& valueTypes = entity->valueTypes;

    if(valueTypes.find("radius") == valueTypes.end())
        throw std::runtime_error("sphere missing radius");
    radius = valueTypes["radius"];


    RayMaterial m;
    if(entity->children.find(MATERIAL) != entity->children.end())
    {
        m = evaluateMaterialEntity(entity->children[MATERIAL]);
    }

    RayObject* sphere = new RaySphere(Vector3D(), radius, m);
    sphere = parseNormalModifier(sphere, entity);
    sphere = parseRotation(sphere, entity);
    sphere = parseTranslation(sphere, entity);

    return sphere;
}

RayObject* Parser::evaluatePlaneEntity(ParseEntity* entity)
{
    Vector3D pos, normal;

    std::map<string, Vector3D>& coordTypes = entity->coordTypes;
    pos = coordTypes["pos"];
    normal = coordTypes["normal"];

    RayMaterial m;
    if(entity->children.find(MATERIAL) != entity->children.end())
    {
        m = evaluateMaterialEntity(entity->children[MATERIAL]);
    }

    RayObject* plane = new RayPlane(pos, normal, m);
    plane = parseNormalModifier(plane, entity);
    plane = parseCheckerTexture(plane, entity);
    return plane;
}
RayObject* Parser::evaluateCylinderEntity(ParseEntity* entity)
{
    double radius = 0;
	double length = 0;

    std::map<string, double>& valueTypes = entity->valueTypes;
    radius = valueTypes["radius"];
    length = valueTypes["length"];

    RayMaterial m;
    if(entity->children.find(MATERIAL) != entity->children.end())
    {
        m = evaluateMaterialEntity(entity->children[MATERIAL]);
    }

    RayObject* cyl = new RayCylinder(radius, length, m);
    cyl = parseRotation(cyl, entity);
    cyl = parseTranslation(cyl, entity);
    cyl = parseNormalModifier(cyl, entity);
    return cyl;
}

RayObject* Parser::evaluateDifferenceEntity(ParseEntity* entity)
{
    std::map<EntityType, ParseEntity*>& children = entity->children;
    
    if(children.size() < 2)
        throw std::runtime_error("difference must have at least two children");

    Difference* diff;
    std::map<EntityType, ParseEntity*>::iterator it = children.begin();
    std::map<EntityType, ParseEntity*>::iterator end = children.end();

    RayObject* r1 = evaluateEntity(it->second);
    ++it;
    RayObject* r2 = evaluateEntity(it->second);
    diff = new Difference(r1, r2);
    ++it;
    while(it != end)
    {
        diff = new Difference(diff, evaluateEntity(it->second)); 
        ++it;
    }     
    return diff;
}


RayObject* Parser::evaluateBoxEntity(ParseEntity* entity)
{
    Vector3D pos;
	Vector3D rot;
    Vector3D size;

    std::map<string, Vector3D>& coordTypes = entity->coordTypes;

    size = coordTypes["size"];

    RayMaterial m;
    if(entity->children.find(MATERIAL) != entity->children.end())
        m = evaluateMaterialEntity(entity->children[MATERIAL]);

    RayObject* box = new RayBox(size.x(), size.y(), size.z(), m);
    box = parseNormalModifier(box, entity);
    box = parseCheckerTexture(box, entity);
    box = parseRotation(box, entity);
    box = parseTranslation(box, entity);

    return box;
}

PointLight* Parser::evaluateLightEntity(ParseEntity* entity)
{
    std::map<string, Vector3D>& coordTypes = entity->coordTypes;
    if(coordTypes.find("pos") == coordTypes.end())
        throw std::runtime_error("light has no pos");
    return new PointLight(coordTypes["pos"]);
}

RayCamera Parser::evaluateCameraEntity(ParseEntity* entity)
{
    m_logger->info("Parsing camera");
    Vector3D location, lookat, up;

    std::map<string, Vector3D>& coordTypes = entity->coordTypes;

    if(coordTypes.find("location") == coordTypes.end())
        throw std::runtime_error("camera has no location");
    location = coordTypes["location"];
    
    if(coordTypes.find("lookat") == coordTypes.end())
        throw std::runtime_error("camera has no lookat");
    lookat = coordTypes["lookat"];

    if(coordTypes.find("up") == coordTypes.end())
        throw std::runtime_error("camera has no up");
    up = coordTypes["up"];

    return RayCamera(RayCamera(location, lookat, up));
}

RayMaterial Parser::evaluateMaterialEntity(ParseEntity* entity)
{
    double ambient 		= 0;
	double diffuse 		= 0;
	double specular     = 0;
	double specpower 	= 0;
	double reflection 	= 0;
	double refraction    = 0;
	double refractionIndex = 0;
    Vector3D color;

    std::map<string, double>& valueTypes = entity->valueTypes;
    std::map<string, Vector3D>& coordTypes = entity->coordTypes;

    // map creates a new default value(0 for double) for keys not in the map
    ambient = valueTypes["ambient"];
    diffuse = valueTypes["diffuse"];
    specular = valueTypes["specular"];
    specpower = valueTypes["specpower"];
    reflection = valueTypes["reflection"];
    refraction = valueTypes["refraction"];
    refractionIndex = valueTypes["refractionIndex"];

    if(coordTypes.find("color") != coordTypes.end())
        color = coordTypes["color"];

    return RayMaterial(RayColor(color.x(),color.y(),color.z()), 
                       ambient, 
                       diffuse, 
                       specular, 
                       specpower,
                       reflection,
					   refraction,
					   refractionIndex);
                
}

RayObject* Parser::parseRotation(RayObject* obj, ParseEntity* entity)
{
    if(entity->coordTypes.find("rot") != entity->coordTypes.end())
        return new Rotation(obj, entity->coordTypes["rot"]);
    return obj;

}

RayObject* Parser::parseTranslation(RayObject* obj, ParseEntity* entity)
{
    if(entity->coordTypes.find("pos") != entity->coordTypes.end())
        return new Translation(obj, entity->coordTypes["pos"]);
    return obj;

}

RayObject* Parser::parseNormalModifier(RayObject* obj, ParseEntity* entity)
{
    std::map<EntityType, ParseEntity*>& children = entity->children;
    if(children.find(NORMAL_MODIFIER) != children.end())
    {
        valueTypes = children[NORMAL_MODIFIER]->valueTypes;
        if(valueTypes.find("amount") == valueTypes.end())
        {
            return obj;
        }
        double amount = valueTypes["amount"];
        return new RandomNormalDisplacer(obj, amount);
    }
    return obj;
}

RayObject* Parser::parseCheckerTexture(RayObject* obj, ParseEntity* entity)
{
    std::map<EntityType, ParseEntity*>& children = entity->children;
    if(children.find(CHECKER) != children.end())
    {
        cout << "CHECKER!!!" << endl;
        coordTypes = children[CHECKER]->coordTypes;
        Vector3D v1 = coordTypes["c1"];
        Vector3D v2 = coordTypes["c2"];
        RayColor c1(v1.x(), v1.y(), v1.z());
        RayColor c2(v2.x(), v2.y(), v2.z());
        return new CheckerTexture(obj, c1, c2);
    }
    return obj;
}
