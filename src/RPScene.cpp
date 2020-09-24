#include "../includes/RPSceneScript.h"
#include "../includes/RPScene.h"
#include "../includes/RPCppTools.h"
#include "../includes/RPEngine.h"

using namespace rp;

//Scene
Scene::Scene()
{
    objsInScene = std::vector<Base*>();
}
void Scene::AddObject(Base* obj)
{
    objsInScene.push_back(obj);
}
void Scene::RemoveObject(int id)
{
    objsInScene.erase(objsInScene.begin()+id);
}
void Scene::RemoveObject(std::string name)
{
    for(unsigned int i = 0; i < objsInScene.size();i++)
    {
        if(objsInScene.at(i)->GetName() == name)
        {
            objsInScene.erase(objsInScene.begin()+i);
        }
    }
}
Base* Scene::GetSceneObject(std::string name)
{
    for(unsigned int i = 0; i < objsInScene.size();i++)
    {
        if(objsInScene.at(i)->GetName() == name)
        {
            return objsInScene.at(i);
            break;
        }
    }
    //return nullptr; //<-- bad idea
    return new Base();
}
void Scene::LogScene(std::vector<Base*>& tmp )
{
    for(unsigned int i = 0; i < tmp.size();i++)
    {
        std::string logStr = "";
        EngineLogger el = EngineLogger();
        logStr += "Name: " + tmp.at(i)->GetName();
        logStr += " X,Y pos: " + std::to_string(tmp.at(i)->GetRect()->x) + "," + std::to_string(tmp.at(i)->GetRect()->y);
        logStr += " Size(w,h) " + std::to_string(tmp.at(i)->GetRect()->w) + "," + std::to_string(tmp.at(i)->GetRect()->h);
        el.Log(logStr);
    }
}

void Scene::SceneUpdate()
{
    std::vector<Base*> tmp = std::vector<Base*>();
    for(unsigned int i = 0; i < objsInScene.size();i++)
    {
        objsInScene.at(i)->Parse(tmp);
    }
    //std::sort(tmp.begin(),tmp.end()); <- commented out as it doesn't sort correctly every time
    //std::cout << "starting sort at " << SDL_GetTicks() << std::endl; //<--- debug
    CppTools::BaseVectorSort(tmp);
    //std::cout << "sorting done at " << SDL_GetTicks() << std::endl; //<--- debug
    
    for(unsigned int i = 0; i < tmp.size();i++)
    {
        if(tmp.at(i)->IsEnabled())
        {
            std::cout << "Rendering object: " << tmp.at(i)->GetName() << " At Z index: " << tmp.at(i)->GetZ() << std::endl;
            for(unsigned int u = 0; i < tmp.at(i)->GetAllScripts().size();u++)
            {
                if(tmp.at(i)->GetAllScripts().at(u)->GetEnabled())
                {
                    tmp.at(i)->GetAllScripts().at(u)->Update(*this);
                }
            }
            tmp.at(i)->Update();
            tmp.at(i)->Draw();
        }
    }
    //LogScene(tmp);
    tmp.clear();
    
}
//Scene::~Scene()
//{
   /* for(unsigned int i = 0; i < objsInScene.size();i++)
    {
      delete(objsInScene.at(i)); //<<--- Deletion causes crashes, when no window is created and program attempts to delete a nullptr due to no scene being created, not even the "protection scene" which is an empty scene that is assigned to the currScene slot, when the window is created - Dumb way to do this already commented out to make space for the good way to do it
    }*/
   
//}
