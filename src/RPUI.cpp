#include "../includes/RPUI.h"
#include <SDL2/SDL_image.h>
using namespace rp;


//Rotation
Rotation::Rotation(float _x, float _y, float _z)
{
    rotX = _x;
    rotY = _y;
    rotZ = _z;
}
Rotation::Rotation()
{
    rotX = 0;
    rotY = 0;
    rotZ = 0;
}

//FullPos2D
FullPos2D::FullPos2D()
{
  x = 0;
  y = 0;
  z = 0;
  rot = Rotation();
}
FullPos2D::FullPos2D(float _x, float _y, float _z)
{
    x = _x;
    y = _y;
    z = _z;
}
FullPos2D::FullPos2D(float _x, float _y, float _z, float _rotationX, float _rotationY, float _RotationZ)
{
    x = _x;
    y = _y;
    z = _z;
    rot = Rotation(_rotationX,_rotationY,_RotationZ);
}

//Base
void Base::SetEnabled(bool state)
{
    enabled = state;
}
bool Base::IsEnabled()
{
    return enabled;
}
void Base::SetName(std::string _name)
{
  name = _name;  
}

std::string Base::GetName()
{
    return name;
}


//UIGraphic
UIGraphic::UIGraphic()
{
    pf = nullptr;
}
UIGraphic::~UIGraphic()
{
  if(pf != nullptr)
  {
      delete(pf);
  }
}
std::string UIGraphic::GetPath()
{
    if(pf != nullptr)
    {
        return pf->GetPath();
    }
    else
    {
      return "";  
    }
}
PngFile* UIGraphic::GetFile()
{
    return pf;
}
UIGraphic::UIGraphic(std::string _path)
{
    LoadGraphic(_path);
}
void UIGraphic::LoadGraphic(std::string path)
{
    pf = new PngFile(path);
}

//UIBase
UIBase::UIBase()
{
    ug = UIGraphic();
}
void UIBase::onClick()
{
    
}
UIBase::UIBase(UIGraphic uig)
{
    SetGraphic(uig);
}
UIBase::UIBase(std::string path)
{
    SetGraphic(path);
}
void UIBase::SetGraphic(UIGraphic uig)
{
    ug = uig;
}
void UIBase::SetGraphic(std::string path)
{
    ug = UIGraphic(path);
}
UIGraphic UIBase::GetGraphic()
{
  return ug;  
}

//UIText
UIText::UIText()
{
  fontPath = defaultFontPath;
  text = "";
  fontSize = defaultFontSize;
  rgb = RGB();
}

void UIText::SetFontSize(int size)
{
  fontSize = size;  
}
int UIText::GetFontSize()
{
  return fontSize;  
}
UIText::UIText(std::string txt)
{
    LoadText(txt);
}
UIText::UIText(std::string path, std::string txt)
{
    LoadText(path,txt);
}
void UIText::LoadText(std::string fontpath, std::string _text)
{
    fontPath = fontpath;
    text = _text;
}
void UIText::LoadText(std::string _text)
{
    text = _text;
}
std::string UIText::GetPath()
{
    return fontPath;
}
std::string UIText::GetText()
{
    return text;
}
void UIText::SetFont(std::string path)
{
    fontPath = path;
}

//Button
Button::Button()
{
    txt = UIText("");
    GetGraphic().LoadGraphic(defaultBtnTexturePath);
}
Button::Button(std::string btnTxt)
{
    txt = UIText(btnTxt);
}
void Button::SetFont(std::string fontpath)
{
  txt.SetFont(fontpath);
}

UIText Button::GetUIText()
{
    return txt;
}
void Button::SetUIText(std::string text)
{
   txt.LoadText(text); 
}
void Button::SetUIText(UIText text)
{
    txt = text;
}

void Button::onClick()
{
    
}

//PosBase
PosBase::PosBase()
{
    rect = new SDL_Rect();
}
SDL_Rect* PosBase::GetRect()
{
    return rect;
}
void rp::PosBase::SetRect(SDL_Rect* _rect)
{
    rect = _rect;
}

//RGB
RGB::RGB()
{
 r = 0;
 g = 0;
 b = 0;
}
RGB::RGB(unsigned int _r, unsigned int _g, unsigned int _b)
{
 r = _r;
 g = _g;
 b = _b;
}

//UIMenu
UIMenu::UIMenu()
{
    
}


