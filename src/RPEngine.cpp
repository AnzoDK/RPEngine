#include "../includes/RPEngine.h"
using namespace rp;
namespace fs = std::filesystem;
#include <fstream>


GameObject::GameObject()
{
    pos = Position(); 
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

AnimationSheet::AnimationSheet(std::string _path)
{
    path = _path;
}

std::vector<CharacterSprite*> AnimationSheet::ProcessSheet(Position frameSize=Position(300,500))
{
  PngFile png = PngFile(path);
  if(png.IsValid())
  {
    
  }
      
}


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
RosenoernEngine::RosenoernEngine(bool _debug,int buffers)
{
    audio = new RosenoernAudio(_debug,buffers);
}
RosenoernEngine::~RosenoernEngine()
{
    delete(audio);
    for(unsigned int i = 0; i < objs.size();i++)
    {
        delete(objs.at(i));
    }
}
void RosenoernEngine::init()
{
    audio->init();
    objs = std::vector<GameObject*>();
}
RosenoernAudio& RosenoernEngine::GetAudioController()
{
 return *audio;   
}
