#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/time.h>

#include "rayworld.h"
#include "raycamera.h"
#include "raysphere.h"
#include "bmp.h"
#include "parser.h"
#include "consolelogger.h"
#include "nologger.h"
#include "consoleprogress.h"

using namespace std;

void printUsage(const string& appname)
{
    cout << "Usage: " << appname << " raytrace-file width height output" << endl;
}

int main(int argc, char** argv)
{   
    if(argc != 5)
    {
        printUsage(argv[0]);
        return 1;
    }
    
    ifstream input(argv[1]);
    if(!input)
    {
        cout << "Error opening input file: " << argv[1] << endl;
        return 2;
    }


    // Read width height
    int width, height;
    istringstream iss(argv[2]);
    if((iss >> width).fail())
    {
        cout << "Error parsing width: " << argv[2] << endl;
        return 3;
    }
    iss.clear();
    iss.str(argv[3]);
    if((iss >> height).fail())
    {
        cout << "Error parsing height: " << argv[3] << endl;
        return 4;
    }
    
    // Open output file
    ofstream file(argv[4]);
    if(!file)
    {
        cout << "Error opening output file: " << argv[4] << endl;
        return 5;
    }

    // Parse
    Progress progress;//(80, width*height);
    RayWorld r(&progress);    
    NoLogger logger;
    Parser p(&r, &logger);
    cout << "Parsing: " << argv[1] << endl;
    try
    {
        p.parse(input);
    } 
    catch(exception& e)
    {
        cout << e.what() << endl;
        cout << "Aborting parser..." << endl;
        return 6;
    }
    
    struct timeval t1, t2;
    gettimeofday(&t1, 0);
    cout << "Rendering..." << endl;
    r.render(width, height);
    gettimeofday(&t2, 0);
    RayCanvas* canvas = r.canvas();    
    long usecdiff = (t2.tv_sec*1000000 + t2.tv_usec) - (t1.tv_sec*1000000 + t1.tv_usec);
    cout << endl << endl << "Render took: " << usecdiff/1000 << " msecs " << endl;

    BMP bmp(width, height);

    for(int x=0;x<width;++x)
    {
        for(int y=0;y<height;++y)
        {
            RayColor col = canvas->color(x,y);
            bmp.setPixelAt(x, y, col.r(), col.g(), col.b());
        }
    }
    file << bmp;

    return 0;
}
