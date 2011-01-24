#include "consoleprogress.h"
#include <string.h>
#include <iostream>

using namespace std;

ConsoleProgress::ConsoleProgress(int width, int max)
    : m_width(width), m_max(max), m_curr(0), m_currbar(-1)
{
}

void ConsoleProgress::tick()
{
    ++m_curr;
    int newbar = ((float) m_curr / m_max )*(float) m_width;
    if(newbar != m_currbar)
    {
        m_currbar = newbar;
        printProgress(newbar);
    } 
}
void ConsoleProgress::printProgress(int bars)
{
    // First clear
    cout << "\r";
    char progressbar[256]; //probably maximum?
    memset(progressbar, 0, 256);
    for (int i = 0; i < m_width; i++) {
        if(i < bars)
            progressbar[i] = '=';
        else
            progressbar[i] = ' ';
    }
    progressbar[m_width] = '\0';
    cout << "[" << progressbar << "]";
    cout.flush();
}
