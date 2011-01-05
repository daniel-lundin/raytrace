#include "consolelogger.h"
#include <iostream>

using std::cout;
using std::endl;

void ConsoleLogger::info(const std::string& str)
{
    cout << "INFO: " << str << endl;
}
void ConsoleLogger::warn(const std::string& str)
{
    cout << "WARN: " << str << endl;
}
void ConsoleLogger::debug(const std::string& str)
{
    cout << "DEBUG: " << str << endl;
}
