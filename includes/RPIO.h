#pragma once
#include <iostream>
#include <fstream>
#include "RPCppTools.h"
#include "RPScene.h"
namespace rp
{






 struct RawFile
 {
     RawFile(){length = 0; path = ""; buffer = new char[5]{'E','M','P','Y'};}
     RawFile(char* buff, int _length,std::string _path){/*buffer = new char[_length];for(int i = 0; i < _length; i++){buffer[i] = buff[i];}*/buffer = buff; length = _length; path = _path;}
     ~RawFile(){delete[] buffer;} //<--- This fucks it up
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
