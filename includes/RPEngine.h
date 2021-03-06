/*
Coded by AnzoDK (https://github.com/AnzoDK) for Rosenørn Productions (rosenoern-productions.dk) Please refer to the LICENCE file for more info about copying and sharing
*/

#define RPEngineVersion "0.2.4.4" 
#pragma once
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
#define Windows
#else
#define Linux
#endif
#include <RPAudio/rpaudio.h>
#include <filesystem>
#if defined(_WIN32) || defined(WIN32)
#include <windows.h>
#include <string>
#endif
#ifndef _WIN32
#endif
#include <iostream>
#include "RPPng.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "RPUI.h"
#include "RPScene.h"
#include "RPRandom.h"
#include <algorithm>
#include "importedClasses.h"
#include "RPSettings.h"
namespace rp{

enum CharacterState{Default=0,Smiling,Crying,Annoyed,Sad,Suprised};
enum AnimationState{Idle=0};
enum RunningState{Stopped=0,Running=1};

//Declare the only global variables we need
static EngineSettings* g_settings;


struct ScreenSize
{
  int width = 0;
  int height = 0;
};
    
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

class SimpleAnimationBase : public Base
{
    public:
        SimpleAnimationBase(){done = false; status=RunningState::Stopped;}
        virtual ~SimpleAnimationBase(){}
        //virtual void Update() override;
        RunningState GetStatus(){return status;}
        void SetStatus(RunningState _state){status = _state;}
        bool IsDone(){return done;}
        void SetDone(bool _done){done = _done;};
    private:
        RunningState status;
        bool done;
        
    
};



class EngineLogger
{
    public:
        EngineLogger(bool withticks=0);
        EngineLogger(bool withticks, std::string path);
        EngineLogger(std::string path);
        void Log(std::string strToLog,bool toConsole=false);
        ~EngineLogger(){}
        void operator <<(std::string TextToLog)
        {
            Log(TextToLog);
        }
        void SetPath(std::string path)
        {
          logPath = path;  
        }
        
    private:
        std::string logPath;
        bool withTicks;
};

static EngineLogger* g_logger; // our primary logger module

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
        
        virtual ~GameObject(){delete(rect); delete(currSprite);}
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


class InputHandler
{
    public:
        InputHandler(){}
        Uint8 GetMouseButton();
        bool MouseButtonDown();
        bool MouseButtonUp();
        SDL_KeyboardEvent GetKeyEvent();
        SDL_Keycode GetKey();
        void SetMouseButton(SDL_Event _evt);
        void SetKey(SDL_Event _evt);
        void Clear();
        void SetUp(){up = true;}
        void SetDown(){down = true;}
    private:
        SDL_Event evt;
        bool up;
        bool down;
};

class RosenoernEngine
{
  public:
  RosenoernEngine(bool _debug=0,int buffers=5);
  ~RosenoernEngine();
  void init();
  void SDLHandle();
  void Quit();
  void SetFPS(int fps);
  RosenoernAudio& GetAudioController();
  int CreateMainWindow(std::string name, Uint32 flags, bool FPSCounter=0);
  static SDL_Window* mainWin;
  static SDL_Renderer* mainRender;
  static int width;
  static int height;
  void SetScene(Scene* s);
  void LoadSceneFromFile(std::string path);
  void Update();
  Base* GetSceneObject(std::string name);
  ScreenSize GetScreenSize();
  bool isRunning;
  static int FPS;
  static int mouseX;
  static int mouseY;
  static InputHandler* InHand;
  void Log(std::string strToLog);
  static Uint32 GetTicks();
  Uint32 GetCurrFps();
  bool FPScounter = 0;
  private:
    //std::sort kept fucking sorting 1 < -1 and THAT IS NOT RIGHT GODDAMMIT
    //Thus - I made a manual sort function
    RosenoernAudio* audio;
    std::vector<GameObject*> objs;
    Scene* currScene;
    int frameDelay;
    EngineLogger logger;
    std::string orgName;
    Uint32 fps_lasttime;
    Uint32 fps_current;
    Uint32 fps_frames;
    bool debug;
    
};



}
