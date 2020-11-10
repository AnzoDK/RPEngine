#include "../includes/RPEngine.h"
#include "../includes/RPCppTools.h"
#include "../includes/RPIni.h"
#include "../includes/RPIO.h"
#include "../includes/RPScene.h"
#include "../includes/RPExceptions.h"
#include <thread>
#include <functional>

using namespace rp;
namespace fs = std::filesystem;
#define MR RosenoernEngine::mainRender
#define _DEBUG_
#include <fstream>
    //To fix some static declare issues
    SDL_Renderer* RosenoernEngine::mainRender = nullptr;
    SDL_Window* RosenoernEngine::mainWin = nullptr;
    int RosenoernEngine::height = 0;
    int RosenoernEngine::width = 0;
    int RosenoernEngine::FPS = 0;
    int RosenoernEngine::mouseX = 0;
    int RosenoernEngine::mouseY = 0;
    InputHandler* RosenoernEngine::InHand = nullptr;
//GameObject
GameObject::GameObject()
{
    rect = new SDL_Rect(); 
}
void GameObject::SetSprite(std::string spritePath)
{
    CharacterSprite* sprite = new CharacterSprite(spritePath);
    currSprite = sprite;
}
void GameObject::SetSprite(CharacterSprite* sprite)
{
    currSprite = sprite;
}
void GameObject::Draw()
{
    if(IsEnabled())
    {
        SDL_Surface* tmpSurf = IMG_Load(currSprite->GetFile()->GetPath().c_str());
        //SDL_Surface* tmpSurf = IMG_Load("testImg.png");
        SDL_Texture* tex = SDL_CreateTextureFromSurface(RosenoernEngine::mainRender,tmpSurf);
        SDL_SetTextureColorMod(tex,TexMod.modR,TexMod.modB,TexMod.modG);
        SDL_SetTextureAlphaMod(tex,TexMod.modR);
        SDL_RenderCopy(RosenoernEngine::mainRender,tex, NULL,rect);
        SDL_FreeSurface(tmpSurf);
        SDL_DestroyTexture(tex);
    }
};
void GameObject::Update()
{
    
    
}

//AnimationSheet
AnimationSheet::AnimationSheet(std::string _path)
{
    path = _path;
}

std::vector<CharacterSprite*> AnimationSheet::ProcessSheet(FullPos2D frameSize=FullPos2D(300,500,0))
{
  PngFile png = PngFile(path);
  if(png.IsValid())
  {
    
  }
      return std::vector<CharacterSprite*>();
}

//SpriteAnimation
SpriteAnimation::SpriteAnimation(int _FPS, std::string animSheet)
{
  playbackFPS = _FPS;
  AnimationSheet sheet = AnimationSheet(animSheet);
  sprites = sheet.ProcessSheet();
}

CharacterSprite::CharacterSprite(std::string path)
{
  file = new PngFile(path);
  if(!file->IsValid())
  {
      //Throw some error at some point
  }
  //Valid :)
}

//SimpleAnimationBase
/*void SimpleAnimationBase::Update()
{
    
}*/


//CharacterSprite
CharacterSprite::~CharacterSprite()
{
  delete(file);
}
PngFile* CharacterSprite::GetFile()
{
  return file;  
}

//CharacterObject
void CharacterObject::Update()
{
    
}

CharacterObject::~CharacterObject()
{
    for(unsigned int i = 0; i < chrStates.size();i++)
    {
        delete(chrStates.at(i));
    }
}

CharacterObject::CharacterObject()
{
 chrStates = std::vector<CharacterSprite*>();
 animations = std::vector<SpriteAnimation*>();
}
void CharacterObject::ChangeState(CharacterState state)
{
  currState = static_cast<int>(state);
}

void CharacterObject::BuildCharacterObject(std::string characterFolder)
{
  for(const auto & entry : fs::directory_iterator(characterFolder))
  {
   CharacterSprite* tmpSprite = new CharacterSprite(entry.path().string());
   chrStates.push_back(tmpSprite);
  }
}

CharacterState CharacterObject::GetState()
{
  return static_cast<CharacterState>(currState);
}

//InputHandler
Uint8 InputHandler::GetMouseButton()
{
    return evt.button.button;
}
SDL_KeyboardEvent InputHandler::GetKeyEvent()
{
   return evt.key;
}
SDL_Keycode InputHandler::GetKey()
{
  return evt.key.keysym.sym;  
}
void InputHandler::SetMouseButton(SDL_Event _evt)
{
    evt = _evt;
}
void InputHandler::Clear()
{
  evt = SDL_Event();
  up = false;
  down = false;
}
void InputHandler::SetKey(SDL_Event _evt)
{
     evt = _evt;
}
bool InputHandler::MouseButtonDown()
{
    return down;
}
bool InputHandler::MouseButtonUp()
{
    return up;
}

//RosenoernEngine
RosenoernEngine::RosenoernEngine(bool _debug,int buffers)
{
    debug = _debug;
    g_settings = new EngineSettings();
    g_logger = new EngineLogger(true);
    try{
        g_settings->LoadConfig();
        g_logger->SetPath(g_settings->GetLogPath());
    }
    catch(InvalidPathException& e)
    {
        //Apply defaults and write new config
        g_logger->SetPath("engineLog.log");
        g_logger->Log("[Settings Module] " + std::string(e.what()),true);
        
    }
    audio = new RosenoernAudio(_debug,buffers);
}
ScreenSize RosenoernEngine::GetScreenSize()
{
  ScreenSize s = ScreenSize(); //Why not just use a fucking rect? or something else like for real this is dumb
  
  SDL_GetWindowSize(RosenoernEngine::mainWin,&s.width,&s.height);
  return s;
}
RosenoernEngine::~RosenoernEngine()
{
    delete(audio);
    for(unsigned int i = 0; i < objs.size();i++)
    {
        delete(objs.at(i));
    }
    SDL_DestroyRenderer(mainRender);
    SDL_DestroyWindow(mainWin);
    delete(currScene);
    delete(InHand);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    delete(g_logger);
    delete(g_settings);
}
void RosenoernEngine::init()
{
    logger = EngineLogger();
    audio->init();
    RosenoernEngine::InHand = new InputHandler();
    isRunning = false;
    currScene = new Scene();
    RosenoernEngine::FPS = 60;
    frameDelay = 1000/RosenoernEngine::FPS;
    /*
     * We are litterally just expecting SDL to work here... Not a good soluton
     */
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init(); // <<-- Important to remember...
    objs = std::vector<GameObject*>();
    orgName = "";
    fps_lasttime = SDL_GetTicks();
    fps_current = 0;
    fps_frames = 0;
    if(debug)
    {
        if(SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING,"RPEngine Debug Mode","RPEngine has been set into debug mode, please remember to launch the program through a terminal in order to get the full debug output",NULL) != 0)
        {
            #ifndef _DEBUG_
            throw GeneralSDLException("Failed to spawn SDL_Messagebox - Messageboxes will be ignored");
            #endif
        }
    }
        
}
RosenoernAudio& RosenoernEngine::GetAudioController()
{
 return *audio;   
}
int RosenoernEngine::CreateMainWindow(std::string windowName, Uint32 flags,bool FPSCounter)
{
    orgName = windowName;
    FPScounter = FPSCounter;
    if(!SDL_CreateWindowAndRenderer(1024,720,flags,&RosenoernEngine::mainWin,&RosenoernEngine::mainRender))
    {
        SDL_SetWindowTitle(RosenoernEngine::mainWin,windowName.c_str());
        SDL_ShowWindow(RosenoernEngine::mainWin);
        SDL_SetRenderDrawColor(RosenoernEngine::mainRender,255,255,255,255);
        SDL_GetWindowSize(RosenoernEngine::mainWin,&RosenoernEngine::width,&RosenoernEngine::height);
        isRunning = 1;
    }
    else
    {
      return 0;  
    }

}
void RosenoernEngine::SDLHandle()
{
    SDL_Event currEvent = SDL_Event();
    InHand->Clear();
    while(SDL_PollEvent(&currEvent))
    {
    switch (currEvent.type) 
    {
        case SDL_WINDOWEVENT:
            switch(currEvent.window.event)
            {
                default:
                break;
                
                case SDL_WINDOWEVENT_CLOSE:
                    Quit();
                break;
            }
        break;
        
        case SDL_MOUSEBUTTONDOWN:
            RosenoernEngine::InHand->SetMouseButton(currEvent);
            RosenoernEngine::InHand->SetDown();
        break;
        
        case SDL_MOUSEBUTTONUP:
            RosenoernEngine::InHand->SetMouseButton(currEvent);
            RosenoernEngine::InHand->SetUp();
        break;
        
        case SDL_KEYDOWN:
            RosenoernEngine::InHand->SetKey(currEvent);
        
        default:
        break;
    }

}
}
void RosenoernEngine::SetScene(Scene* s)
{
  delete(currScene);
  currScene = s;
}

void RosenoernEngine::LoadSceneFromFile(std::string path)
{
    SetScene(RPIO::LoadScene(path));  
}
void RosenoernEngine::Update()
{
    #ifdef Windows
    uint32_t frameStart = 0;
    #else
    u_int32_t frameStart = 0;
    #endif
    int frameTime = 0;
    frameStart = SDL_GetTicks();
    SDL_GetMouseState(&mouseX,&mouseY);
    //std::bind(&RosenoernEngine::SDLHandle,this);
    SDLHandle();
    //std::thread handleThread(std::bind(&RosenoernEngine::SDLHandle,this));
    SDL_RenderClear(MR);
    currScene->SceneUpdate();
    SDL_RenderPresent(MR);

    frameTime = SDL_GetTicks() - frameStart;
    //std::cout << "Frametime: " << std::to_string(frameTime) << std::endl;
    fps_frames++;
    if (fps_lasttime < SDL_GetTicks() - 1.0*1000)
    {
      fps_lasttime = SDL_GetTicks();
      fps_current = fps_frames;
      fps_frames = 0;
    }
    if(FPScounter)
    {
        std::string newName = orgName;
        newName += " [FPS: ";
        //newName += std::to_string(/*float(*/frameTime/*)*/);
        newName += std::to_string(/*float(*/fps_current/*)*/);
        newName += "]";
        SDL_SetWindowTitle(mainWin,newName.c_str());
    }
    if(frameDelay > frameTime)
    {
        SDL_Delay(frameDelay-frameTime);
        //std::cout << "Frame delayed" << std::endl;
    }
}

Uint32 RosenoernEngine::GetCurrFps()
{
    return fps_current;   
}

void RosenoernEngine::Quit()
{
    isRunning = 0;   
}
void RosenoernEngine::SetFPS(int fps)
{
  RosenoernEngine::FPS = fps;
  frameDelay = 1000/fps;
}

void RosenoernEngine::Log(std::string strToLog)
{
    logger.Log(strToLog);
}
Base* RosenoernEngine::GetSceneObject(std::string name)
{
  return currScene->GetSceneObject(name);  
}

Uint32 RosenoernEngine::GetTicks()
{
    return SDL_GetTicks();  
}

EngineLogger::EngineLogger(bool withticks)
{
  logPath = "./GameLog.log";
  withTicks = withticks;
}
EngineLogger::EngineLogger(std::string path)
{
    logPath = path;
}
EngineLogger::EngineLogger(bool withticks, std::string path)
{
    logPath = path;
    withTicks = withticks;
}
void EngineLogger::Log(std::string strToLog, bool toConsole)
{
    std::string logStr = "";
    if(withTicks)
    {
        logStr = "[" + std::to_string(RosenoernEngine::GetTicks()) + "] ";
    }
    logStr += strToLog;
    std::ofstream out = std::ofstream(logPath.c_str(),std::fstream::out | std::fstream::app);
    out << logStr;
    out << std::endl;
    out.close();
    if(toConsole)
    {
        std::cout << logStr << std::endl;
    }
    
}


//Easy Rect Creation
SDL_Rect* CreateRect(int x, int y, int w, int h)
{
  SDL_Rect* r = new SDL_Rect(); 
  r->x = x;
  r->y = y;
  r->w = w;
  r->h = h;
  return r;
}


//More UIstuff - mainly drawing


//UIMenu Draw
/*void UIMenu::Draw()
{
    SDL_Surface* tmpSurf = IMG_Load(bg->GetFile()->GetPath().c_str());
    SDL_Texture* tex = SDL_CreateTextureFromSurface(RosenoernEngine::mainRender,tmpSurf);
    SDL_RenderCopy(RosenoernEngine::mainRender,tex, NULL,GetRect());
    SDL_FreeSurface(tmpSurf);
    SDL_DestroyTexture(tex);
}*/
