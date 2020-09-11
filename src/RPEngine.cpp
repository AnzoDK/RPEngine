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
Base* RosenoernEngine::GetObject(std::string name)
{
  return currScene->GetObject(name);  
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
Base* Scene::GetObject(std::string name)
{
    for(unsigned int i = 0; i < objsInScene.size();i++)
    {
        if(objsInScene.at(i)->GetName() == name)
        {
            return objsInScene.at(i);
            break;
        }
    }
    return nullptr;
}
void Scene::LogScene(std::vector<Base*>& tmp )
{
    for(unsigned int i = 0; i < tmp.size();i++)
    {
        std::string logStr = "";
        EngineLogger el = EngineLogger();
        logStr += "Name: " + tmp.at(i)->GetName();
        logStr += " X,Y pos: " + std::to_string(tmp.at(i)->GetRect()->x) + "," + std::to_string(tmp.at(i)->GetRect()->y);
        logStr += " Size(w,h) " + std::to_string(tmp.at(i)->GetRect()->w) + "," + std::to_string(tmp.at(i)->GetRect()->h);
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
    //std::sort(tmp.begin(),tmp.end()); <- commented out as it doesn't sort correctly every time
    CppTools::BaseVectorSort(tmp);
    
    for(unsigned int i = 0; i < tmp.size();i++)
    {
        if(tmp.at(i)->IsEnabled())
        {
            std::cout << "Rendering object: " + tmp.at(i)->GetName() + " At Z index: " + std::to_string(tmp.at(i)->GetZ()) << std::endl;
            tmp.at(i)->Update();
            tmp.at(i)->Draw();
        }
    }
    //LogScene(tmp);
    tmp.clear();
    
}
//Scene::~Scene()
//{
   /* for(unsigned int i = 0; i < objsInScene.size();i++)
    {
      delete(objsInScene.at(i)); //<<--- Deletion causes crashes, when no window is created and program attempts to delete a nullptr due to no scene being created, not even the "protection scene" which is an empty scene that is assigned to the currScene slot, when the window is created - Dumb way to do this already commented out to make space for the good way to do it
    }*/
   
//}
Scene* Scene::LoadScene(std::string path)
{
    //Not a good way - Should just have a static logger...
    EngineLogger el = EngineLogger();
    //signed char* cannot find 0xFF. We have to work around it. 
    unsigned char* objectEnd = 0;
    unsigned char* objectStart = 0;
    RawFile sceneFile = RPIO::ReadFileRaw(path);
    //Check if header is sane
    char* expectedHeader = new char[3]{0x52,0x50,0x53};
    int ver = 0;
    Scene* s = new Scene();
    ver = ver << sceneFile.buffer[3] << 8 | sceneFile.buffer[4];
    switch(ver)
    {
        case 1:
            //Default version
            objectEnd = new unsigned char[2]{0x0F,0xF0};
            objectStart = new unsigned char[2]{0xF0,0x0F};
            
            //sanity check
            if((sceneFile.buffer[0] << 16 | sceneFile.buffer[1] << 8 | sceneFile.buffer[2]) == (expectedHeader[0] << 16 | sceneFile.buffer[1] << 8 | expectedHeader[2]))
            {
                el.Log("LoadScene - INFO: File is sane");
                DoubleArray<int>* pairs = new DoubleArray<int>();
                ArrPair<unsigned char*> patterns = ArrPair<unsigned char*>(objectStart,objectEnd);
                LoadPairs(*pairs,sceneFile,patterns);
                std::string name = "";
                UIBase* tmpBase = 0;
                SDL_Rect* tmpRect = 0;
                for(int i = 0; i < pairs->length();i++)
                {
                    for(int u = 0; u < pairs->at(i).item2-pairs->at(i).item1;u++)
                    {
                    tmpBase = new UIBase();
                    tmpRect = new SDL_Rect();
                    int nameLengh = 0;                    
                    if(sceneFile.buffer[pairs->at(i).item1+u] == (char)0xAA/* && sceneFile.buffer[i+1] == (char)0x01*/)
                    {
                        switch(sceneFile.buffer[pairs->at(i).item1+u+1])
                        {
                            default:
                                
                            break;
                            
                            case (char)0x01:
                                nameLengh = sceneFile.buffer[pairs->at(i).item1+u+2];
                                for(int c = 0; c < nameLengh;c++)
                                {
                                    name += sceneFile.buffer[pairs->at(i).item1+u+3+c];
                                }
                                tmpBase->SetName(name);
                            break;
                            
                            case (char)0x02:
                                    tmpRect->x = (sceneFile.buffer[pairs->at(i).item1+u+2] << 8 | sceneFile.buffer[pairs->at(i).item1+u+3]);
                            break;
                            
                            case (char)0x03:
                                    tmpRect->y = (sceneFile.buffer[pairs->at(i).item1+u+2] << 8 | sceneFile.buffer[pairs->at(i).item1+u+3]);
                            break;
                            case (char)0x04:
                                    tmpRect->w = (sceneFile.buffer[pairs->at(i).item1+u+2] << 8 | sceneFile.buffer[pairs->at(i).item1+u+3]);
                            break;
                            
                            case (char)0x05:
                                    tmpRect->h = (sceneFile.buffer[pairs->at(i).item1+u+2] << 8 | sceneFile.buffer[pairs->at(i).item1+u+3]);
                            break;
                            
                            case (char)0x06:
                                    std::string uiPath = "";
                                    for(int c = 0; c < sceneFile.buffer[pairs->at(i).item1+u+2];c++)
                                    {
                                        uiPath += sceneFile.buffer[pairs->at(i).item1+u+3+c];
                                    }
                                    tmpBase->SetGraphic(new UIGraphic(uiPath));
                            break;
                        }
                    }
                }
                }
                
            }
            else
            {
               el.Log("LoadScene - ERROR: File is insane(unexpected value in header)"); 
            }
        break;
        
        case 2:
            
        break;
        
        default:

            el.Log("LoadScene - Error: Scene file version unexpected, corrupted file?");
        break;
    }
    if(objectEnd != 0)
    {
        delete[] objectEnd;
        delete[] objectStart;
    }
}
void Scene::LoadPairs(DoubleArray<int>& dvec, RawFile file, ArrPair<unsigned char*> filePatterns)
{
    int objStart = -1;
    int objEnd = -1;
    for(int i = 0; i < file.length;i++)
    {
        char* workBytes = new char[2];
        workBytes[i] = file.buffer[i];
        workBytes[i+1] = file.buffer[i+1];
        if(CppTools::CharArrToUnsginedArr(workBytes,2) == filePatterns.item1)
        {
            objStart = i;
        }
        if(CppTools::CharArrToUnsginedArr(workBytes,2) == filePatterns.item2)
        {
            objEnd = i;
        }
        if(objStart != -1 && objEnd != -1)
        {
            dvec.Add(objStart,objEnd);
            objStart = -1;
            objEnd = -1;
        }
        
    }
}
Uint32 RosenoernEngine::GetTicks()
{
    return SDL_GetTicks();  
}

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
    std::ofstream out = std::ofstream(logPath.c_str(),std::fstream::out | std::fstream::app);
    out << logStr;
    out << std::endl;
    out.close();
    
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
