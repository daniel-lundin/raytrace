#include <iostream>
#include <fstream>
#include <sstream>
#include "rayworld.h"
#include "raycamera.h"
#include "raysphere.h"
#include "bmp.h"
#include "parser.h"
#include "consolelogger.h"

using namespace std;

void printUsage(const string& appname)
{
    cout << "Usage: " << appname << " raytrace-file output" << endl;
}

int main(int argc, char** argv)
{   
    if(argc != 2)
    {
        printUsage(argv[0]);
        return 1;
    }
    
    ifstream input(argv[1]);
    if(!input)
    {
        cout << "Bad file" << endl;
        return 2;
    }


    // Parse
    RayWorld r;    
    ConsoleLogger logger;
    Parser p(&r, &logger);
    try
    {
        p.parse(input);
    } catch(exception& e)
    {
        cout << "Parse error: " << e.what() << endl;
    }
    
    // Ray trace
    RayCamera cam;
    cam.setLocation(Vector3D(0,2,0));
    cam.setLookat(Vector3D(0,0,10));
    cam.setUp(Vector3D(0,1,0));    

    int width = 400;
    int height = 300;

    r.render(width, height);
    RayCanvas* canvas = r.canvas();    
    BMP bmp(width, height);

    for(int x=0;x<width;++x)
    {
        for(int y=0;y<height;++y)
        {
            RayColor col = canvas->color(x,y);
            bmp.setPixelAt(x, y, col.r(), col.g(), col.b());
        }
    }
    ofstream file("out.bmp");
    file << bmp;


    return 0;
}
