#include "../includes/RPEngine.h"
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
#define Windows
#else
#define Linux
#endif
using namespace rp;
namespace fs = std::filesystem;
#define MR RosenoernEngine::mainRender
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
SDL_MouseButtonEvent InputHandler::GetMouseButton()
{
    return evt.button;
}
SDL_KeyboardEvent InputHandler::GetKey()
{
    
   return evt.key; 
}
void InputHandler::SetMouseButton(SDL_Event _evt)
{
    evt = _evt;
}
void InputHandler::Clear()
{
  evt = SDL_Event();  
}
void InputHandler::SetKey(SDL_Event _evt)
{
     evt = _evt;
}


//RosenoernEngine
RosenoernEngine::RosenoernEngine(bool _debug,int buffers)
{
    audio = new RosenoernAudio(_debug,buffers);
}
ScreenSize RosenoernEngine::GetScreenSize()
{
  ScreenSize s = ScreenSize();
  
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
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init(); // <<-- Important to remember...
    objs = std::vector<GameObject*>();
}
RosenoernAudio& RosenoernEngine::GetAudioController()
{
 return *audio;   
}
int RosenoernEngine::CreateMainWindow(std::string windowName, Uint32 flags)
{
    if(!SDL_CreateWindowAndRenderer(1024,720,flags,&RosenoernEngine::mainWin,&RosenoernEngine::mainRender))
    {
        SDL_SetWindowTitle(RosenoernEngine::mainWin,windowName.c_str());
        SDL_ShowWindow(RosenoernEngine::mainWin);
        SDL_SetRenderDrawColor(RosenoernEngine::mainRender,255,0,0,255);
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
    SDLHandle();
    SDL_RenderClear(MR);
    currScene->SceneUpdate();
    SDL_RenderPresent(MR);

    frameTime = SDL_GetTicks() - frameStart;
    //std::cout << "Frametime: " << std::to_string(frameTime) << std::endl;
    if(frameDelay > frameTime)
    {
        SDL_Delay(frameDelay-frameTime);
    }
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

void RosenoernEngine::Log(std::string strToLog, bool withTicks)
{
    logger.Log(strToLog,withTicks);
}


//Scene
Scene::Scene()
{
    objsInScene = std::vector<Base*>();
}
void Scene::AddObject(Base* obj)
{
    objsInScene.push_back(obj);
}
void Scene::RemoveObject(int id)
{
    objsInScene.erase(objsInScene.begin()+id);
}
void Scene::RemoveObject(std::string name)
{
    for(unsigned int i = 0; i < objsInScene.size();i++)
    {
        if(objsInScene.at(i)->GetName() == name)
        {
            objsInScene.erase(objsInScene.begin()+i);
        }
    }
}
void Scene::LogScene()
{
    for(unsigned int i = 0; i < objsInScene.size();i++)
    {
        std::string logStr = "";
        EngineLogger el = EngineLogger();
        logStr += "Name: " + objsInScene.at(i)->GetName();
        logStr += " X,Y pos: " + std::to_string(objsInScene.at(i)->GetRect()->x) + "," + std::to_string(objsInScene.at(i)->GetRect()->y);
        logStr += " Size(w,h) " + std::to_string(objsInScene.at(i)->GetRect()->w) + "," + std::to_string(objsInScene.at(i)->GetRect()->h);
        el.Log(logStr);
    }
}

void Scene::SceneUpdate()
{
    std::vector<Base*> tmp = std::vector<Base*>();
    for(unsigned int i = 0; i < objsInScene.size();i++)
    {
        objsInScene.at(i)->Parse(tmp);
    }
    std::sort(tmp.begin(),tmp.end());
    
    for(unsigned int i = 0; i < tmp.size();i++)
    {
        if(tmp.at(i)->IsEnabled())
        {
            std::cout << "Rendering object: " + tmp.at(i)->GetName() + " At Z index: " + std::to_string(tmp.at(i)->GetZ()) << std::endl;
            tmp.at(i)->Update();
            tmp.at(i)->Draw();
        }
    }
    tmp.clear();
    
}
Scene::~Scene()
{
   /* for(unsigned int i = 0; i < objsInScene.size();i++)
    {
      delete(objsInScene.at(i)); //<<--- Deletion causes crashes, when no window is created and program attempts to delete a nullptr due to no scene being created, not even the "protection scene" which is an empty scene that is assigned to the currScene slot, when the window is created - Dumb way to do this already commented out to make space for the good way to do it
    }*/
   objsInScene.clear();
}

  #ifdef Windows
  uint32_t GetTicks()
  {
      return SDL_GetTicks();
  }
  #else
  u_int32_t GetTicks()
  {
    return SDL_GetTicks();  
  }
  #endif

EngineLogger::EngineLogger()
{
  logPath = "./GameLog.log";
}
EngineLogger::EngineLogger(std::string path)
{
    logPath = path;
}
void EngineLogger::Log(std::string strToLog, bool withTicks)
{
    std::string logStr = "";
    if(withTicks)
    {
        logStr = "[" + std::to_string(RosenoernEngine::GetTicks()) + "] ";
    }
    logStr += strToLog;
    std::ofstream out = std::ofstream(logPath.c_str());
    out << logStr;
    out.close();
    
}



//More UIstuff - mainly drawing
//Button Draw
void Button::Draw()
{
  if(IsEnabled())
    {
        /*std::cout << "Path for resource is: " + GetGraphic()->GetFile()->GetPath() << std::endl;*/
        SDL_Surface* tmpSurf = IMG_Load(GetGraphic()->GetFile()->GetPath().c_str());
        //SDL_Surface* tmpSurf = IMG_Load("testImg.png");
        SDL_Texture* tex = SDL_CreateTextureFromSurface(RosenoernEngine::mainRender,tmpSurf);
        SDL_RenderCopy(RosenoernEngine::mainRender,tex, NULL,GetRect());
        SDL_FreeSurface(tmpSurf);
        SDL_DestroyTexture(tex);
        SDL_Rect* rr = new SDL_Rect();
        rr->h = GetRect()->h*0.8;
        rr->w = GetRect()->w*0.8;
        rr->x = GetRect()->x + (GetRect()->w/8);
        rr->y = GetRect()->y + (GetRect()->h/8);
        GetUIText()->SetRect(rr);
        GetUIText()->Draw();
    }  
}
//Button Update
void Button::Update()
{
    
    if(RosenoernEngine::mouseX > GetRect()->x && RosenoernEngine::mouseX < (GetRect()->w+GetRect()->x) && RosenoernEngine::mouseY > GetRect()->y && RosenoernEngine::mouseY < (GetRect()->h+ GetRect()->y))
    {
        if(RosenoernEngine::InHand->GetMouseButton().button == SDL_BUTTON_LEFT)
        {
            //std::cout << "Clicked!" << std::endl;
            (*funPtr)();
        }
        //onHover();
    }
}

//UIText Draw
void UIText::Draw()
{
    //std::cout << "Text:" << text << " Font: " + fontPath + " Size: " + std::to_string(fontSize) << std::endl;
    TTF_Font* font = TTF_OpenFont(fontPath.c_str(),fontSize);
    SDL_Color clr = {static_cast<Uint8>(rgb->r),static_cast<Uint8>(rgb->g),static_cast<Uint8>(rgb->b),static_cast<Uint8>(rgb->a)};
    SDL_Surface* surf = TTF_RenderText_Solid(font, text.c_str(), clr);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(RosenoernEngine::mainRender,surf);
    SDL_FreeSurface(surf);
    SDL_RenderCopy(RosenoernEngine::mainRender,tex,NULL,GetRect());
    SDL_DestroyTexture(tex);
    TTF_CloseFont(font); // <<--- Also very Important. If this isn't done it will crash after a few updates
    
    
}
//UIMenu Draw
/*void UIMenu::Draw()
{
    SDL_Surface* tmpSurf = IMG_Load(bg->GetFile()->GetPath().c_str());
    SDL_Texture* tex = SDL_CreateTextureFromSurface(RosenoernEngine::mainRender,tmpSurf);
    SDL_RenderCopy(RosenoernEngine::mainRender,tex, NULL,GetRect());
    SDL_FreeSurface(tmpSurf);
    SDL_DestroyTexture(tex);
}*/
//Background
Background::Background()
{
    SDL_Rect* _rect = new SDL_Rect();
    _rect->h = RosenoernEngine::height;
    _rect->w = RosenoernEngine::width;
    SetRect(_rect);
}
Background::Background(std::string path)
{
    SDL_Rect* _rect = new SDL_Rect();
    _rect->h = RosenoernEngine::height;
    _rect->w = RosenoernEngine::width;
    SetRect(_rect);
    SetZ(-1);
    SetGraphic(path);
}

//Background Draw
void Background::Draw()
{
    if(IsEnabled())
    {
        SDL_Surface* tmpSurf = IMG_Load(GetGraphic()->GetFile()->GetPath().c_str());
        //SDL_Surface* tmpSurf = IMG_Load("testImg.png");
        SDL_Texture* tex = SDL_CreateTextureFromSurface(RosenoernEngine::mainRender,tmpSurf);
        SDL_RenderCopy(RosenoernEngine::mainRender,tex, NULL,GetRect());
        SDL_FreeSurface(tmpSurf);
        SDL_DestroyTexture(tex);
    }  
}
//ButtonImage Draw
void ButtonImage::Draw()
{
        SDL_Surface* tmpSurf = IMG_Load(GetGraphic()->GetFile()->GetPath().c_str());
        //SDL_Surface* tmpSurf = IMG_Load("testImg.png");
        SDL_Texture* tex = SDL_CreateTextureFromSurface(RosenoernEngine::mainRender,tmpSurf);
        SDL_RenderCopy(RosenoernEngine::mainRender,tex, NULL,GetRect());
        SDL_FreeSurface(tmpSurf);
        SDL_DestroyTexture(tex);
}
