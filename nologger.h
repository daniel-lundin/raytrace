#ifndef NOLOGGER_H
#define NOLOGGER_H

#include "loginterface.h"
#include <string>

class NoLogger : public Logger
{
public:
    void info(const std::string& s){};
    void warn(const std::string& s){};
    void debug(const std::string& s){};
};

#endif
