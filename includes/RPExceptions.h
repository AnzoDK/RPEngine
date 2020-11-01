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
    std::string err;
    static std::string* s;
    GeneralSDLException(const char* info)
    {
        err = info;
    }
    GeneralSDLException()
    {
        err = "";
    }
    
    const char* what() const throw (){
        if(err == "")
        {
            std::string* s = new std::string("One or more SDL features failed a task. SDL_Error: " + std::string(SDL_GetError())); //Possible memleak
            return s->c_str();
        }
        else
        {
            std::string* s = new std::string(err + std::string(SDL_GetError())); //Possible memleak
            return s->c_str();
        }
    }
    ~GeneralSDLException()
    {
      delete(GeneralSDLException::s);  
    }
};
