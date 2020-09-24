#pragma once
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
        private:
            //static DerivedRegister<SceneScriptBase> reg;
            std::string name;
            
    };
}
