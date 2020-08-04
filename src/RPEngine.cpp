#include "../includes/RPEngine.h"
using namespace rp;
namespace fs = std::filesystem;
#include <fstream>


GameObject::GameObject()
{
    pos = Position(); 
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
  std::ifstream reader = std::ifstream(path,std::ios::binary | std::ios::ate);
  unsigned int bufferSize = reader.tellg();
  char* ch_buffer = new char[bufferSize];
  reader.read(ch_buffer,bufferSize);
  reader.close();
  buffer = new uint8_t[bufferSize];
  for(int i = 0; i < bufferSize;i++)
  {
    buffer[i] = ch_buffer[i];
  }
  delete[] ch_buffer;
}

CharacterSprite::~CharacterSprite()
{
  delete[] buffer;  
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
 std::vector<CharacterSprite*> chrStates = std::vector<CharacterSprite*>(); 
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
