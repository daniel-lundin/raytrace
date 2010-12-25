#ifndef __BMP__
#define __BMP__
#include <iostream>
#include <fstream>

using namespace std;

struct pixel24
{
	unsigned char r;
	unsigned char g;
	unsigned char b;	
};



class BMP
{
public:
    BMP(const int width, const int height);
    ~BMP();
    
    void generateShit();
    
    friend ostream& operator<<(ostream& os, const BMP&);
    
private:
    int m_width;
    int m_height;
    pixel24* m_pixelArray;
};

ostream& operator<<(ostream& os, const BMP&);

#endif

