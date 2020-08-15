#include "../includes/RPEngine.h"
using namespace rp;
namespace fs = std::filesystem;
#define MR RosenoernEngine::mainRender
#include <fstream>
    //To fix some static declare issues
    SDL_Renderer* RosenoernEngine::mainRender = nullptr;
    SDL_Window* RosenoernEngine::mainWin = nullptr;
    int RosenoernEngine::height = 0;
    int RosenoernEngine::width = 0;
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
   CharacterSprite* tmpSprite = new CharacterSprite(entry.path());
   chrStates.push_back(tmpSprite);
  }
}

CharacterState CharacterObject::GetState()
{
  return static_cast<CharacterState>(currState);
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
    IMG_Quit();
    SDL_Quit();
}
void RosenoernEngine::init()
{
    audio->init();
    isRunning = false;
    currScene = new Scene();
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG);
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
    SDL_PollEvent(&currEvent);
    switch (currEvent.type) 
    {
        case SDL_WINDOWEVENT:
            switch(currEvent.window.event)
            {
                default:
                break;
                
                case SDL_WINDOWEVENT_CLOSE:
                    isRunning = 0;
                break;
            }
        break;
        
        default:
        break;
    }

}
void RosenoernEngine::SetScene(Scene* s)
{
  delete(currScene);
  currScene = s;
}
void RosenoernEngine::Update()
{
    
    SDL_RenderClear(MR);
    currScene->SceneUpdate();
    SDLHandle();
    SDL_RenderPresent(MR);
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
void Scene::SceneUpdate()
{
    for(unsigned int i = 0; i < objsInScene.size();i++)
    {
        objsInScene.at(i)->Update();
        objsInScene.at(i)->Draw();
    }   
}
Scene::~Scene()
{
    for(unsigned int i = 0; i < objsInScene.size();i++)
    {
      delete(objsInScene.at(i));  
    }
}

//More UIstuff - mainly drawing
//Button Draw
void Button::Draw()
{
  if(IsEnabled())
    {
        SDL_Surface* tmpSurf = IMG_Load(GetGraphic().GetFile()->GetPath().c_str());
        //SDL_Surface* tmpSurf = IMG_Load("testImg.png");
        SDL_Texture* tex = SDL_CreateTextureFromSurface(RosenoernEngine::mainRender,tmpSurf);
        SDL_RenderCopy(RosenoernEngine::mainRender,tex, NULL,GetRect());
        SDL_FreeSurface(tmpSurf);
        SDL_DestroyTexture(tex);
        SDL_Rect* rr = new SDL_Rect();
        rr->h = GetRect()->h*0.8;
        rr->w = GetRect()->w*0.8;
        rr->x = GetRect()->x*0.1+GetRect()->x;
        rr->x = GetRect()->y*0.1+GetRect()->y;
        GetUIText().SetRect(rr);
        GetUIText().Draw();
    }  
}
//UIText Draw
void UIText::Draw()
{
    TTF_Font* font = TTF_OpenFont(GetPath().c_str(),GetFontSize());
    SDL_Color clr = {static_cast<Uint8>(rgb.r),static_cast<Uint8>(rgb.g),static_cast<Uint8>(rgb.b)};
    SDL_Surface* surf = TTF_RenderText_Solid(font, GetText().c_str(), clr);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(RosenoernEngine::mainRender,surf);
    SDL_FreeSurface(surf);
    SDL_RenderCopy(RosenoernEngine::mainRender,tex,NULL,GetRect());
    SDL_DestroyTexture(tex);
    
    
}

