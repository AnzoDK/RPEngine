#pragma once
#include <iostream>
//#include "RPUI.h"
namespace rp
{
    class SceneScriptBase
    {
        public:
            SceneScriptBase();
            virtual ~SceneScriptBase(){}
            virtual void Update();
            void SetName(std::string _name);
            std::string GetName(){return name;}
            void SetEnabled(bool enableState){enabled = enableState;}
            bool GetEnabled(){return enabled;}
        private:
            //static DerivedRegister<SceneScriptBase> reg;
            std::string name;
            bool enabled;
            
    };
}
