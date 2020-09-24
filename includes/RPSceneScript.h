#pragma once
#include "RPUI.h"
namespace rp
{
    class SceneScriptBase : public Base
    {
        public:
            SceneScriptBase(){SetName("New SceneScriptBase");/*derrivedClass = "SceneScriptBase";*/}
            void Update() override{}
            void Draw() override{};
        private:
            //static DerivedRegister<SceneScriptBase> reg;
            
    };
}
