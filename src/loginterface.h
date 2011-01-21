#ifndef LOGINTERFACE_H
#define LOGINTERFACE_H
#include <string>

class Logger
{
public:
    virtual void info(const std::string&) = 0;
    virtual void warn(const std::string&) = 0;
    virtual void debug(const std::string&) = 0;
};
#endif // LOGINTERFACE_H
