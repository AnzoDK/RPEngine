#include <iostream>
#include "../includes/RPSettings.h"
#include "../includes/RPExceptions.h"
#include <fstream>
using namespace rp;

EngineSettings::EngineSettings()
{
    //Use default path meaning enginesettings.ini in the executeable directory
    settingsPath = "enginesettings.ini";
}
EngineSettings::EngineSettings(std::string settings_path)
{
    settingsPath = settings_path;
}
void EngineSettings::SetLogPath (std::string p)
{
    logPath = p;
}
            
std::string EngineSettings::GetLogPath()
{
    return logPath;  
}

void EngineSettings::LoadConfig()
{
    std::fstream stream;
    stream.open(settingsPath);
    if(stream.fail())
    {
        throw new InvalidPathException();
    }
    else
    {
        config = new Ini(settingsPath);
    }
    
}
