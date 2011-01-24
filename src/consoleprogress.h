#ifndef CONSOLEPROGRESS_H
#define CONSOLEPROGRESS_H

#include "progress.h"

class ConsoleProgress : public Progress
{
public:
    ConsoleProgress(int width, int max);
    void tick();
private:
    void printProgress(int bars);

    int m_width;
    int m_max;
    int m_curr;
    int m_currbar;
};

#endif
