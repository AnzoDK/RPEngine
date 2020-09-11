#pragma once
#include "RPUI.h"

namespace rp
{
class Scene
{
    public:
        Scene();
        virtual ~Scene(){objsInScene.clear();}
        void AddObject(Base* obj);
        void RemoveObject(int id);
        void RemoveObject(std::string name);
        Base* GetObject(std::string name);
        void SceneUpdate();
        
        
    private:
        
        void LogScene(std::vector<Base*>& tmp);
        std::vector<Base*> objsInScene;
        
   
};
}
