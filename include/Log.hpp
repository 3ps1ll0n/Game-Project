#pragma once

#include <fstream>
#include <map>

class Log
{
private:
    static std::string path;
public:
    static void clearFile(std::string file);
    static void writeLog(std::string file, std::string line);
};
