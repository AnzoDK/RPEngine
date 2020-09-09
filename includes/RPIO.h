#pragma once
#include <iostream>
#include <fstream>

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
    private:
        
 };
}
