#pragma once
#include "../includes/RPAudio.h"
#ifdef _WIN32
#include <windows.h>
#endif
#ifndef
#endif
#include <iostream>
namespace rp{

struct CharacterSprite
{
  public:
    std::string path;
    
}

enum CharacterState{Default=0,Smiling,Crying,Annoyed,Sad};

class CharacterObject
{
  public:
    ChangeState(CharacterState state);
    ~CharacterObject();
  private:
    int currState;
    //Holds the characters sprites for different states - Should always be ordered in the same way as CharacterState (i.e Default sprite would be in slot 0 and so on)
    std::vector<CharacterSprite> chrStates;
}

class Scene
{

};


class RosenoernEngine
{
  public:
  RosenoernEngine();
  ~RosenoernEngine();
  init();
  private:
    RosenoernAudio* audio;
};



}
