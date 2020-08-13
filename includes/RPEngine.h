#pragma once
#include "RPAudio/rpaudio.h"
#include <filesystem>
#ifdef _WIN32
#include <windows.h>
#endif
#ifndef _WIN32
#endif
#include <iostream>
#include "RPPng.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
namespace rp{

struct Position
{
    template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
    Position(T _x, T _y)
    {
      x = _x;
      y = _y;
    }
    Position()
    {
      x = 0;
      y = 0;
    }
    float x,y;
};

    
struct CharacterSprite
{
  public:
    CharacterSprite(std::string path);
    std::string path;
    ~CharacterSprite();
  private:
    uint8_t* buffer;
    
};

struct AnimationSheet
{
    public:
        AnimationSheet(std::string _path);
        std::vector<CharacterSprite*> ProcessSheet(Position frameSize);
    private:
        std::string path;
};

struct SpriteAnimation
{
    public:
        SpriteAnimation(int _FPS, std::string animSheet);
        
    private:
        std::vector<CharacterSprite*> sprites;
        int playbackFPS;
};



enum CharacterState{Default=0,Smiling,Crying,Annoyed,Sad,Suprised};

class GameObject
{
    public:
        GameObject();
        template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
        GameObject(T x, T y)
        {
            pos = Position(x,y);
        }
        
        virtual ~GameObject(){}
        virtual void Update(){}
    private:
        Position pos;
};


class CharacterObject : GameObject
{
  public:
    //Takes a folder and reads all files in the folder as sprites - as of now it uses the CharacterState as namingscheme. so 0.png would be default for the character
    CharacterObject();
    ~CharacterObject();
    void BuildCharacterObject(std::string characterFolder);
    void ChangeState(CharacterState state);
    CharacterState GetState();
    void Update() override;
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
  RosenoernEngine(bool _debug=0,int buffers=5);
  ~RosenoernEngine();
  void init();
  RosenoernAudio& GetAudioController();
  int CreateMainWindow(std::string name, Uint32 flags);
  
  
  private:
    SDL_Window* mainWin;
    SDL_Renderer* mainRender;
    RosenoernAudio* audio;
    std::vector<GameObject*> objs;
    
};



}
