#include "../includes/RPEngine.h"
using namespace rp;
namespace fs = std::filesystem;
#include <fstream>

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
RosenoernEngine::RosenoernEngine()
{
    
}
RosenoernEngine::~RosenoernEngine()
{
    delete(audio);
}
void RosenoernEngine::init()
{
    audio = new RosenoernAudio(0,5);
    audio->init();
}

