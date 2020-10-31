#pragma once
#include <iostream>
#include "RPIni.h"
namespace rp
{
    struct EngineSettings
    {
        private:
            std::string logPath;
            std::string settingsPath; //Expects an .ini file for settings
            Ini* config;
            
            
        public:
            EngineSettings();
            EngineSettings(std::string settings_path);
            void SetLogPath (std::string p);
            std::string GetLogPath();
            void LoadConfig();
            
    };
}
