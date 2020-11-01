#pragma once
#include <iostream>
#include <SDL2/SDL.h>
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
struct GeneralSDLException: public std::exception
{
    const char* what() const throw (){
        std::string* s = new std::string("One or more SDL features failed a task. SDL_Error: " + std::string(SDL_GetError())); //Possible memleak
        return s->c_str();
    }
};
