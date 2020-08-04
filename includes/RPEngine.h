#pragma once
#include "RPAudio/rpaudio.h"
#include <filesystem>
#ifdef _WIN32
#include <windows.h>
#endif
#ifndef _WIN32
#endif
#include <iostream>
namespace rp{

struct CharacterSprite
{
  public:
    CharacterSprite(std::string path);
    std::string path;
  private:
    uint8_t* buffer;
    
};

enum CharacterState{Default=0,Smiling,Crying,Annoyed,Sad};

class CharacterObject
{
  public:
    //Takes a folder and reads all files in the folder as sprites - as of now it uses the CharacterState as namingscheme. so 0.png would be default for the character
    void BuildCharacterObject(std::string characterFolder);
    CharacterObject();
    void ChangeState(CharacterState state);
    CharacterState GetState();
    ~CharacterObject();
  private:
    int currState;
    //Holds the characters sprites for different states - Should always be ordered in the same way as CharacterState (i.e Default sprite would be in slot 0 and so on)
    std::vector<CharacterSprite*> chrStates;
};

class Scene
{

};


class RosenoernEngine
{
  public:
  RosenoernEngine();
  ~RosenoernEngine();
  void init();
  RosenoernAudio& GetAudioController();
  
  private:
    RosenoernAudio* audio;
};



}
