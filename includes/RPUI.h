#pragma once
#include <iostream>
#include "RPPng.h"
#include <SDL2/SDL_ttf.h>
#define defaultFontPath "Resources/fonts/default.ttf"
#define defaultFontSize 24
#define defaultBtnTexturePath "Resources/textures/btnDefault.png"
namespace rp
{
    enum CommonColor{White,Red,Green,Blue,Black};
    struct RGB
    {
        RGB();
        RGB(CommonColor cc);
        RGB(unsigned int _r, unsigned int _g, unsigned int _b, unsigned int _a);
        unsigned int r;
        unsigned int g;
        unsigned int b;
        unsigned int a;
    };
    
    struct Rotation
    {
        Rotation(float _x, float _y, float _z);
        Rotation();
        float rotX;
        float rotY;
        //Dont use the rotZ - I don't even know why it's here
        float rotZ;
    };
    
    struct FullPos2D
    {
        FullPos2D();
        FullPos2D(float _x, float _y, float _z);
        FullPos2D(float _x, float _y, float _z, float _rotationX, float _rotationY, float _RotationZ);
        float x;
        float y;
        //z will not be shown, but is there to help determine what order the objects should be rendered in.
        float z;
        Rotation rot;
        
    };
    
    class PosBase
    {
        public:
            PosBase();
            virtual ~PosBase(){/*delete(rect);*/ /*<<--- Seems like SDL auto clears these...*/}
            SDL_Rect* GetRect();
            void SetRect(SDL_Rect* _rect);
        private:
            SDL_Rect* rect;
            
    };
    
    class Base : public PosBase
    {
        public:
            Base(){enabled = true;}
            virtual ~Base(){}
            virtual void Update(){}
            virtual void Draw(){}
            virtual void Init(){}
            bool IsEnabled();
            void SetEnabled(bool state);
            std::string GetName();
            void SetName(std::string name);
        private:
            bool enabled;
            std::string name;
            
    };
    
    
    class UIText : public Base
    {
        public:
            UIText();
            UIText(std::string fontpath, std::string text);
            UIText(std::string text);
            void LoadText(std::string fontpath, std::string text);
            void SetFont(std::string fontpath);
            //Loads text with default font
            void LoadText(std::string text);
            std::string GetPath();
            std::string GetText();
            void Draw() override;
            void SetFontSize(int size);
            int GetFontSize();
            void SetTextColor(int r, int g, int b);
            void SetTextColor(int r, int g, int b, int a);
            void SetTextColor(CommonColor cc);
            RGB rgb;
        private:
            std::string fontPath;
            std::string text;
            int fontSize;
    };
    
    class UIGraphic
    {
        public:
            UIGraphic();
            UIGraphic(std::string path);
            ~UIGraphic();
            //Return path if pf is initilized - otherwise returns ""
            std::string GetPath();
            //May return nullptr if no PngFile is loaded - Usually it is tho
            PngFile* GetFile();
            void LoadGraphic(std::string path);
        private:
            PngFile* pf;
    };
    
    class UIBase : public Base
    {
        public:
            UIBase();
            virtual ~UIBase(){delete(ug);}
            UIBase(UIGraphic* uig);
            UIBase(std::string path);
            void SetGraphic(UIGraphic* uig);
            void SetGraphic(std::string path);
            UIGraphic* GetGraphic();
            virtual void onClick();
        private:
            UIGraphic* ug;
            
            
    };

    class Button : public UIBase
    {
        public:
            Button();
            virtual ~Button(){}
            Button(std::string btnTxt);
            void Draw() override;
            void onClick() override;
            UIText GetUIText();
            void SetUIText(UIText txt);
            void SetUIText(std::string txt);
            void SetFont(std::string fontpath);
        private:
            UIText txt;
            
        
      
    };
        class UIMenu : public Base //Could also be called a canvas I guess
    {
        public:
            UIMenu();
            virtual ~UIMenu(){}
        private:
            
    };
}

