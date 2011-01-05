#ifndef NOLOGGER_H
#define NOLOGGER_H

#include "loginterface.h"

class NoLogger : public LogInterface
{
public:
    void info(const std::string& s){}
    void warn(const std::string& s){}
    void debug(const std::string& s){}
};

#endif
