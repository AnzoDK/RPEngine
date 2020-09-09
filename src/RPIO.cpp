#include "../includes/RPIO.h"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <limits>
#include <memory>

using namespace rp;

RawFile RPIO::ReadFileRaw(std::string path)
{
    std::ifstream ifs = std::ifstream(path, std::ios::binary);
    ifs.ignore( std::numeric_limits<std::streamsize>::max() );
    std::streamsize length = ifs.gcount();
    ifs.clear();
    ifs.seekg( 0, std::ios_base::beg );
    
    char* buffer = new char[length];
    ifs.read(buffer,length);
    ifs.close();
    return RawFile(buffer,length);
    
}
