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

#include "RPUI.h"
namespace rp{

enum CharacterState{Default=0,Smiling,Crying,Annoyed,Sad,Suprised};
enum AnimationState{Idle=0};

    
struct CharacterSprite
{
  public:
    CharacterSprite(std::string path);
    std::string path;
    ~CharacterSprite();
    PngFile* GetFile();
  private:
    PngFile* file;
    
};

struct AnimationSheet
{
    public:
        AnimationSheet(std::string _path);
        std::vector<CharacterSprite*> ProcessSheet(FullPos2D frameSize);
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





class GameObject : public Base
{
    public:
        GameObject();
        template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
        GameObject(T x, T y, T h, T w)
        {
            rect = new SDL_Rect();
            rect->h = h;
            rect->w = w;
            rect->x = x;
            rect->y = y;
        }
        
        virtual ~GameObject(){delete(rect);}
        virtual void Draw() override;
        virtual void Update() override;
        void SetSprite(CharacterSprite* sprite);
        void SetSprite(std::string spritePath);
    private:
        SDL_Rect* rect;
        CharacterSprite* currSprite;
};


class CharacterObject : public GameObject
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
    std::vector<SpriteAnimation*> animations;
};

class Scene
{
    public:
        Scene();
        ~Scene();
        void AddObject(Base* obj);
        void RemoveObject(int id);
        void RemoveObject(std::string name);
        void SceneUpdate();
    private:
        std::vector<Base*> objsInScene;
   
};


class RosenoernEngine
{
  public:
  RosenoernEngine(bool _debug=0,int buffers=5);
  ~RosenoernEngine();
  void init();
  void SDLHandle();
  RosenoernAudio& GetAudioController();
  int CreateMainWindow(std::string name, Uint32 flags);
  static SDL_Window* mainWin;
  static SDL_Renderer* mainRender;
  void SetScene(Scene* s);
  void Update();
  bool isRunning;
  
  
  private:
    RosenoernAudio* audio;
    std::vector<GameObject*> objs;
    Scene* currScene;
    
};



}
