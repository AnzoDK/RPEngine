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
    if(argc != 2)
    {
        std::cout << "No arguement for 'scriptPath' was given" << std::endl
        << "USAGE: RPScriptLinker [scriptPath]" << std::endl;
        return 1;
    }
    std::string scriptPath = argv[1];
    std::string writePath = "../includes/importedClasses.h";
    WriteInitialSetup(writePath);
    std::ofstream of = std::ofstream(writePath, std::ios::app | std::ios::out);
    for(const auto & entry : fs::directory_iterator(scriptPath))
    {
        if(std::string(entry.path().string()).find(".h") != std::string::npos)
        {
            std::cout << "Linking " << entry.path() << " to RPEngine" << std::endl;
            of << "#include " << '"' << entry.path() << '"' << std::endl;
            
        }
    }
    std::cout << "GameScript Linking Complete!" << std::endl;
    of.close();
    
    return 0;
}
