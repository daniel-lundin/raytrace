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
    void setPixelAt(int x, int y, unsigned char r, unsigned char g, unsigned char b);
    
    friend ostream& operator<<(ostream& os, const BMP&);
    
private:
    int m_width;
    int m_height;
    pixel24* m_pixelArray;
};

ostream& operator<<(ostream& os, const BMP&);

#endif

