#pragma once
#include <iostream>
#include <fstream>

namespace rp
{
 class RPIO
 {
    public:
        RPIO();
        ~RPIO();
        static void ReadFileRaw(std::string path, char*& buf);
    private:
        
 };
}
