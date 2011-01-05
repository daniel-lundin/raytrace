#ifndef CONSOLELOGGER_H
#define CONSOLELOGGER_H

#include <string>
#include "loginterface.h"

class ConsoleLogger : public Logger
{
public:
    void info(const std::string&);
    void warn(const std::string&);
    void debug(const std::string&);
};
#endif
