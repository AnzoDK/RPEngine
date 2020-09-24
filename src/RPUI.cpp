#include "../includes/RPUI.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include "../includes/RPEngine.h"
using namespace rp;

void SimpleTextureSystem::SimpleRender(UIBase& obj)
{
    if(obj.IsEnabled())
    {
        if(obj.GetTexture() == nullptr)
        {
            SDL_Surface* tmpSurf = IMG_Load(obj.GetGraphic()->GetFile()->GetPath().c_str());
            SDL_Texture* tex = SDL_CreateTextureFromSurface(RosenoernEngine::mainRender,tmpSurf);
            SDL_SetTextureColorMod(tex,obj.TexMod.modR,obj.TexMod.modB,obj.TexMod.modG);
            SDL_SetTextureAlphaMod(tex,obj.TexMod.modA);
            SDL_RenderCopy(RosenoernEngine::mainRender,tex, NULL,obj.GetRect());
            SDL_FreeSurface(tmpSurf);
            obj.SetTexture(tex);
        }
        else
        {
            SDL_SetTextureColorMod(obj.GetTexture(),obj.TexMod.modR,obj.TexMod.modB,obj.TexMod.modG);
            SDL_SetTextureAlphaMod(obj.GetTexture(),obj.TexMod.modA);
            SDL_RenderCopy(RosenoernEngine::mainRender,obj.GetTexture(), NULL,obj.GetRect());
        }
        
    } 
}

//Static definitions
//BaseFactory::map_type* BaseFactory::map = NULL;

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
void rp::Base::SetZ(float _z)
{
   z = _z; 
}
float Base::GetZ()
{
    return z;
}

SceneScriptBase* Base::GetScript(std::string name)
{
    for(unsigned int i = 0; i < scripts.size();i++)
    {
        if(scripts.at(i)->GetName() == name)
        {
            return scripts.at(i);
            
        }
    }
    return nullptr;
}
SceneScriptBase* Base::GetScript(int index)
{
    if(index > scripts.size()-1)
    {
        /*Invalid Index*/
        return nullptr;
        
    }
    else
    {
        return scripts.at(index);
        
    }
    
}
void Base::RunScripts()
{
  for(unsigned int i = 0; i < scripts.size();i++)
  {
      if(scripts.at(i)->GetEnabled())
      {
        scripts.at(i)->Update();
      }
  }
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
    if(pf != nullptr)
    {
        return pf;
    }
    pf = new PngFile("");
    return pf;
}
UIGraphic::UIGraphic(std::string _path)
{
    pf = new PngFile(_path);
}
void UIGraphic::LoadGraphic(std::string path)
{
    if(pf != nullptr)
    {
      delete pf;  
    }
    pf = new PngFile(path);
    
}

//UIBase
UIBase::UIBase()
{
    ug = new UIGraphic();
    texture = nullptr;
    SetName("new UIBase");
}
void UIBase::onClick()
{
    
}
void UIBase::Draw()
{
    SimpleTextureSystem::SimpleRender(*this);
}
UIBase::UIBase(UIGraphic* uig)
{
    SetGraphic(uig);
}
UIBase::UIBase(std::string path)
{
    SetGraphic(path);
}
void UIBase::SetGraphic(UIGraphic* uig)
{
    delete(ug);
    ug = uig;
}
void UIBase::SetGraphic(std::string path)
{
    delete(ug);
    ug = new UIGraphic(path);
}
UIGraphic* UIBase::GetGraphic()
{
  return ug;  
}



//UIText
UIText::UIText()
{
  fontPath = defaultFontPath;
  text = "";
  fontSize = defaultFontSize;
  rgb = new C_RGB();
  texture = nullptr;
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
    fontPath = defaultFontPath;
    text = "";
    fontSize = defaultFontSize;
    rgb = new C_RGB();
    LoadText(txt);
    texture = nullptr;
}
UIText::UIText(std::string path, std::string txt)
{
    fontSize = defaultFontSize;
    rgb = new C_RGB();
    LoadText(path,txt);
    texture = nullptr;
    
}
UIText::UIText(std::string fontpath, std::string text, int _fontSize, int x, int y, int width, int height)
{
    fontSize = _fontSize;
    rgb = new C_RGB();
    LoadText(fontpath,text);
    SDL_Rect* rr = new SDL_Rect();
    rr->x = x;
    rr->y = y;
    rr->h = height;
    rr->w = width;
    SetRect(rr);
    texture = nullptr;
}

void UIText::LoadText(std::string fontpath, std::string _text)
{
    if(texture != nullptr)
    {
        SDL_DestroyTexture(texture);
    }
    texture = nullptr;
    fontPath = fontpath;
    text = _text;
}
void UIText::LoadText(std::string _text)
{
    if(texture != nullptr)
    {
        SDL_DestroyTexture(texture);
    }
    texture = nullptr;
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
void UIText::SetTextColor(int r, int g, int b)
{
    rgb = new C_RGB(r,g,b,255);
}
void UIText::SetTextColor(int r, int g, int b, int a)
{
    rgb = new C_RGB(r,g,b,a);
}
void UIText::SetTextColor(CommonColor cc)
{
    rgb = new C_RGB(cc);
}
void UIText::SetFont(std::string path)
{
    fontPath = path;
}


//ButtonImage
ButtonImage::ButtonImage(std::string imgPath)
{
    SetGraphic(imgPath);
}


//Button
Button::Button()
{
    txt = new UIText();
    void (*ptr)(){&Button::empty};
    funPtr = ptr;
    SetGraphic(defaultBtnTexturePath);
}
Button::Button(std::string btnTxt)
{
    void (*ptr)(){&Button::empty};
    funPtr = ptr;
    txt = new UIText(btnTxt);
    SetGraphic(defaultBtnTexturePath);
}
void Button::SetFont(std::string fontpath)
{
  txt->SetFont(fontpath);
}

UIText* Button::GetUIText()
{
    return txt;
}
void Button::SetUIText(std::string text)
{
   txt->LoadText(text); 
}
void Button::SetUIText(UIText* text)
{
    delete(txt);
    txt = text;
}

void Button::onClick()
{
    
}
void Button::SetFunction(void (*funptr)())
{
    funPtr = funptr;
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
    if(rect != nullptr)
    {
        //delete(rect);
    }
    rect = _rect;
}

//RGB
C_RGB::C_RGB()
{
 r = 0;
 g = 0;
 b = 0;
 a = 0;
}
C_RGB::C_RGB(unsigned int _r, unsigned int _g, unsigned int _b, unsigned int _a)
{
 r = _r;
 g = _g;
 b = _b;
 a = _a;
}
C_RGB::C_RGB(CommonColor cc)
{
    switch(static_cast<int>(cc))
    {
        case CommonColor::White:
            r = 255;
            g = 255;
            b = 255;
        break;
        
        case CommonColor::Black:
            a = 255;
            r = 0;
            b = 0;
            g = 0;
        break;
        
        case CommonColor::Red:
            r = 255;
            g = 0;
            b = 0;
        break;
        
        case CommonColor::Green:
            g = 255;
            r = 0;
            b = 0;
        break;
        
        case CommonColor::Blue:
            b = 225;
            r = 0;
            g = 0;
        break;
    }
    a = 255;
}

//UIMenu
UIMenu::UIMenu()
{
    
}
//Background
Background::Background()
{
    SDL_Rect* _rect = new SDL_Rect();
    _rect->h = RosenoernEngine::height;
    _rect->w = RosenoernEngine::width;
    SetRect(_rect);
}
Background::Background(std::string path)
{
    SDL_Rect* _rect = new SDL_Rect();
    _rect->h = RosenoernEngine::height;
    _rect->w = RosenoernEngine::width;
    SetRect(_rect);
    SetZ(-1);
    SetGraphic(path);
}
//UIText Draw
void UIText::Draw()
{
    if(IsEnabled())
    {
        if(texture == nullptr)
        {
            TTF_Font* font = TTF_OpenFont(fontPath.c_str(),fontSize);
            SDL_Color clr = {static_cast<Uint8>(rgb->r),static_cast<Uint8>(rgb->g),static_cast<Uint8>(rgb->b),static_cast<Uint8>(rgb->a)};
            SDL_Surface* surf = TTF_RenderText_Solid(font, text.c_str(), clr);
            SDL_Texture* tex = SDL_CreateTextureFromSurface(RosenoernEngine::mainRender,surf);
            SDL_SetTextureColorMod(tex,TexMod.modR,TexMod.modB,TexMod.modG);
            SDL_SetTextureAlphaMod(tex,TexMod.modA);    
            SDL_FreeSurface(surf);
            SDL_RenderCopy(RosenoernEngine::mainRender,tex,NULL,GetRect());
            texture = tex;
            TTF_CloseFont(font); // <<--- Also very Important. If this isn't done it will crash after a few updates
        }
        else
        {
            SDL_SetTextureColorMod(texture,TexMod.modR,TexMod.modB,TexMod.modG);
            SDL_SetTextureAlphaMod(texture,TexMod.modA);
            SDL_RenderCopy(RosenoernEngine::mainRender,texture,NULL,GetRect());
        }
    }
}
//ButtonImage Draw
void ButtonImage::Draw()
{
        SimpleTextureSystem::SimpleRender(*this);
}
//Button Update
void Button::Update()
{
    if(IsEnabled())
    {
        if(RosenoernEngine::mouseX > GetRect()->x && RosenoernEngine::mouseX < (GetRect()->w+GetRect()->x) && RosenoernEngine::mouseY > GetRect()->y && RosenoernEngine::mouseY < (GetRect()->h+ GetRect()->y))
        {
            if(RosenoernEngine::InHand->GetMouseButton().button == SDL_BUTTON_LEFT)
            {
                //std::cout << "Clicked!" << std::endl;
                (*funPtr)();
            }
            //onHover();
        }
    }
}
//Button Draw
void Button::Draw()
{
  if(IsEnabled())
    {
        SimpleTextureSystem::SimpleRender(*this);
        SDL_Rect* rr = new SDL_Rect();
        rr->h = GetRect()->h*0.8;
        rr->w = GetRect()->w*0.8;
        rr->x = GetRect()->x + (GetRect()->w/8);
        rr->y = GetRect()->y + (GetRect()->h/8);
        GetUIText()->SetRect(rr);
        GetUIText()->Draw();
    }  
}


//Registers
//Lets forget that idea
/*
DerivedRegister<Button> Button::reg("Button");
DerivedRegister<Background> Background::reg("Background");
DerivedRegister<UIBase> UIBase::reg("UIBase");
DerivedRegister<UIText> UIText::reg("UIText");
*/
