#pragma once
#include <iostream>
#include "RPPng.h"
#define defaultFontPath "Resources/fonts/default.ttf"
namespace rp
{
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
    
    class Base
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
    
    class UIMenu : public Base //Could also be called a canvas I guess
    {
        
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
        private:
            std::string fontPath;
            std::string text;
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
            UIBase(UIGraphic uig);
            UIBase(std::string path);
            void SetGraphic(UIGraphic uig);
            void SetGraphic(std::string path);
            virtual void onClick();
        private:
            UIGraphic ug;
            
    };

    class Button : public UIBase
    {
        public:
            Button();
            Button(std::string btnTxt);
            void onClick() override;
            UIText GetUIText();
            void SetFont(std::string fontpath);
        private:
            UIText txt;
        
      
    };
}
