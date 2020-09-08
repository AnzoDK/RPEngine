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
        static char* ReadFileRaw(std::string path);
    private:
        
 };
}
