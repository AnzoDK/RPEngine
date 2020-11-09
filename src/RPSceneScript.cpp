#include "../includes/RPSceneScript.h"
using namespace rp;

SceneScriptBase::SceneScriptBase()
{
    SetName("New SceneScriptBase");
    enabled = true;
}
void SceneScriptBase::SetName(std::string _name)
{
    name = _name;
}

//DerivedRegister<SceneScriptBase> SceneScriptBase::reg("SceneScriptBase");
