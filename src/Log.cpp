#include "Log.hpp"

std::string Log::path = "log/";

void Log::clearFile(std::string file){
    std::ofstream f(path + file);
    f.close();
}

void Log::writeLog(std::string file, std::string line)
{
    std::ofstream logFile(path + file, std::ios_base::app);
    logFile << line << std::endl;
    logFile.close();
}