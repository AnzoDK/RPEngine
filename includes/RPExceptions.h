#pragma once
#include <iostream>
struct InvalidPathException : public std::exception
{
    const char* what() const throw (){
        return "Invalid path or no file found exception";
    }
};

struct InvalidConfigException : public std::exception
{
    const char* what() const throw (){
        return "Config file contains invalid data";
    }
};
