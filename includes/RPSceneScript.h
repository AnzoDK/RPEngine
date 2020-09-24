#pragma once
#include "RPUI.h"
namespace rp
{
    class SceneScriptBase : public Base
    {
        public:
            SceneScriptBase();
            virtual ~SceneScriptBase(){}
            virtual void Update() override;
            virtual void Draw() override;
        private:
            //static DerivedRegister<SceneScriptBase> reg;
            
    };
}
