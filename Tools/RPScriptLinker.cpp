#include <iostream>
//#include "../includes/RPEngine.h"
#include <fstream>
//Needs c++17
#include <filesystem>
namespace fs = std::filesystem;


void WriteInitialSetup(std::string path)
{
    std::ofstream of = std::ofstream(path, std::ios::out);
    of << "#pragma once" << std::endl << "#include " << '"' << "RPEngine.h" << '"' << std::endl;
    of.close();
}

int main(int argc, char *argv[])
{
    //Load all .h classes into importedClasses.h
    if(argc != 3)
    {
        std::cout << "No arguement for 'scriptPath' or 'installpath' was given" << std::endl
        << "USAGE: RPScriptLinker [scriptPath] [RPEnginePath]" << std::endl;
        return 1;
    }
    std::string scriptPath = argv[1];
    std::string installPath = argv[2];
    std::string writePath = installPath + "/importedClasses.h";
    WriteInitialSetup(writePath);
    fs::create_directory(scriptPath); // <-- This is just to make sure that we don't access an nonexistant directory
    fs::create_directory(installPath); // <-- This is just to make sure that we don't access an nonexistant directory
    std::ofstream of = std::ofstream(writePath, std::ios::app | std::ios::out);
    for(const auto & entry : fs::directory_iterator(scriptPath))
    {
        if(std::string(entry.path().string()).find(".h") != std::string::npos || std::string(entry.path().string()).find(".cpp") != std::string::npos) 
        {
            std::cout << "Linking " << entry.path() << " to RPEngine" << std::endl;
            of << "#include " << fs::absolute(entry.path()) << std::endl; //Only works on linux
            
        }
    }
    std::cout << "GameScript Linking Complete!" << std::endl;
    of.close();
    
    return 0;
}
