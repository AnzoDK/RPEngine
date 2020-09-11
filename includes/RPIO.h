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
     RawFile(char* buff, int _length){buffer = buff; length = _length;}
     ~RawFile(){delete[] buffer;}
     char* buffer;
     int length;
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
