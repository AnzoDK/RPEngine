#include "../includes/RPEngine.h"
using namespace rp;
#ifdef __CPP17__
namespace fs = std::filesystem;
#endif
#include <fstream>

RPEngine::CharacterSprite::CharacterSprite(std::string path)
{
  std::ofstream reader = std::ofstream(path,std::ios::binary | std::ios::ate);
  unsinged int bufferSize = reader.tellg();
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

RPEngine::CharacterObject::CharacterObject()
{
 std::vector<CharacterSprite*> chrStates = std::vector<CharacterSprite*>(); 
}
void RPEngine::CharacterObject::ChangeState(CharacterState state)
{
  currState = static_cast<int>(state);
}

void RPEngin::CharacterObject::BuildCharacterObject(std::string characterFolder)
{
  //Requires std=c++17
  #ifdef __CPP17__
  for(const auto & entry : fs::directory_iterator(characterFolder))
  {
   CharacterSprite* tmpSprite = new CharacterSprite(p.path());
   chrStates.push_back(tmpSprite);
  }
  #endif
}

CharacterState RPEngine::CharacterObject::GetState()
{
  return static_cast<CharacterState>(currState);
}
