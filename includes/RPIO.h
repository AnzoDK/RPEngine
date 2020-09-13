#pragma once
#include <iostream>
#include <fstream>
#include "RPCppTools.h"
#include "RPScene.h"
namespace rp
{






 struct RawFile
 {
     RawFile(){}
     RawFile(char* buff, int _length,std::string _path){buffer = buff; length = _length; path = _path;}
     ~RawFile(){/*delete[] buffer;*/ /*<-- Causes invalid delete - Still wondering why..*/ /*Testing other delete --->*/ delete(buffer);}
     char* buffer;
     int length;
     std::string path;
 };
    
 class RPIO
 {
    public:
        RPIO();
        ~RPIO();
        static RawFile ReadFileRaw(std::string path);
        static void LoadPairs(DoubleArray<int>& dvec, RawFile file, ArrPair<unsigned char*> filePatterns);
        static Scene* LoadScene(std::string path);
    private:
        
 };
}
