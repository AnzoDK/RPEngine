#pragma once
#include <iostream>
#include <filesystem>

/*The (hopefully) only place to check OS and lib requirements*/ /*Yea no*/
#if defined(_WIN32)
#include <windows.h>
#endif

#if defined(__linux__) || defined(__MINGW32__)
#include <X11/X.h>
#include <X11/Xlib.h>
#define X11
#endif




namespace fs = std::filesystem;

/*------------Define End------------*/

/*---------Exceptions Start---------*/

struct BadPairLookup : public std::exception
{
    
    const char* what() const throw()
    {
      return "Search for variable in NamedPair failed - Nothing matched string";  
    }
};

struct BadFunctionPtr : public std::exception
{
    const char* what() const throw()
    {
      return "The provided pointer to a function isn't valid";  
    }
};

/*----------Exceptions End----------*/

/*-----------Enums Start------------*/

enum EngineMode{FunctionMode,ClassMode};

/*------------Enums End-------------*/

template<typename T>
struct NamedPair
{
    NamedPair(T Item1, std::string Name1, T Item2, std::string Name2){item1 = Item1; item2 = Item2; name1 = Name1; name2 = Name2;}
    NamedPair(){}
    T operator()(std::string itemName)
    {
        std::string search = std::string(name1+name2);
        if(search.find(itemName) != std::string::npos)
        {
            if(search.find(itemName) >= name1.length())
            {
                return item1;
            }
            else
            {
                return item2;
            }
            
        }
        else
        {
            throw new BadPairLookup;
        }
        
    }
    T item1;
    T item2;
    std::string name1;
    std::string name2;
};

//This Struct is not gonna handle C arrays at all (Please don't use them with this struct)
template <typename T1, typename T2>
struct DoubleItem
{
    DoubleItem(T1 t1, T2 t2){item1 = t1; item2 = t2;}
    bool operator==(DoubleItem di2){if(di2.item1 == item1 && di2.item2 == item2){return true;}else{return false;}}
    DoubleItem& operator=(DoubleItem di2){item1 = di2.item1;item2 = di2.item2; return **this;}
    ~DoubleItem(){}
    T1 item1;
    T2 item2;
};

template <typename T>
struct Vector2D
{
    T x,y;
    Vector2D(T X, T Y)
    {
        x = X;
        y = Y;
    }
    
    void Add(Vector2D v2)
    {
        x += v2.x;
        y += v2.y;
    }
    
    void Subtract(Vector2D v2)
    {
        x -= v2.x;
        y -= v2.y;
    }
    
    Vector2D operator+(Vector2D v2)
    {
        return Vector2D(x+v2.x,y+v2.y);
    }
    
    Vector2D operator-(Vector2D v2)
    {
        return Vector2D(x-v2.x,y-v2.y);
    }
    
    Vector2D& operator+=(Vector2D v2)
    {
        x += v2.x;
        y += v2.y;
        return **this;
    }
    
    Vector2D& operator-=(Vector2D v2)
    {
        x -= v2.x;
        y -= v2.y;
        return **this;
    }
    
    
    
};

struct Rect
{
  Rect(){}
  Rect(float X, float Y, float W, float H){x = X; y = Y; w = W; h = H;}
  float x,y,w,h;  
};


struct Point2D
{
    Point2D(float X, float Y){x = X; y = Y;}
    Point2D(){}
    float x,y;
};

struct Color
{
  Color(uint8_t R, uint8_t G, uint8_t B)
  {
      r = R;
      g = G;
      b = B;
  }
  static Color* ColorCopy(Color c)
  {
     return new Color(c.r,c.g,c.b); 
  }
  uint8_t r,g,b;  
};


class RPEngine
{
    public:
        RPEngine(int width, int height);
        RPEngine(int width, int height, void(*updateFunction)(), void (*initFunction)());
        void Start();
        void Stop();
        void DrawLine(Point2D point1, Point2D point2);
        void DrawLine(float x1, float y1, float x2, float y2);
        void DrawShape(Point2D points[], int length);
        void Fill(Color c);
        void Fill(uint8_t R, uint8_t G, uint8_t B);
        virtual void Run(){}
        virtual void Init(){}
        virtual ~RPEngine();
    private:
        DoubleItem<int, int>* m_screenSize;
        #ifdef X11
        Display* m_xDisplay;
        Window m_xWindow;
        int m_xScreen;
        NamedPair<int> m_displaySize;
        XSetWindowAttributes m_windowAttributes;
        GC m_context;
        #endif
        void(*m_funcUpdatePtr)();
        void (*m_funcInitPtr)();
        void m_StartLoop();
        bool m_bRunning;
        EngineMode m_mode;
        Color* m_colorInUse;
};

RPEngine::RPEngine(int width, int height)
{
    
    m_screenSize = new DoubleItem<int, int>(width,height);
    m_displaySize = NamedPair<int>();
    m_funcUpdatePtr = nullptr;
    m_funcInitPtr = nullptr;
    m_mode = EngineMode::ClassMode;
}
RPEngine::RPEngine(int width, int height, void(*updateFunction)(), void (*initFunction)())
{
    m_screenSize = new DoubleItem<int, int>(width,height);
    m_displaySize = NamedPair<int>();
    m_funcUpdatePtr = updateFunction;
    m_funcInitPtr = initFunction;
    m_mode = EngineMode::FunctionMode;
    
}
RPEngine::~RPEngine()
{
    delete(m_screenSize);
    #ifdef X11
    XDestroyWindow(m_xDisplay,m_xWindow);
    XCloseDisplay(m_xDisplay);
    XFree(m_context);
    #endif
}
void RPEngine::Start()
{
    #ifdef X11
    m_xDisplay = XOpenDisplay(NULL); //Default display
    m_xScreen = DefaultScreen(m_xDisplay);
    m_context = XDefaultGC(m_xDisplay,m_xScreen);
    m_windowAttributes.background_pixel = XWhitePixel(m_xDisplay,m_xScreen); //Background of window, when nothing is drawn
    m_windowAttributes.border_pixel = XBlackPixel(m_xDisplay, m_xScreen); //Border color
    m_displaySize = NamedPair<int>(XDisplayWidth(m_xDisplay,m_xScreen),"width",XDisplayHeight(m_xDisplay,m_xScreen),"height"); //Just the size of the display
    m_xWindow = XCreateWindow(m_xDisplay,
                              DefaultRootWindow(m_xDisplay), //Find the root (the display itself)
                              1, 1, //x,y coordinates for the new window
                              m_screenSize->item1,m_screenSize->item2, //The width and height for the window
                              2, //the border thickness
                              CopyFromParent, //The bit depth of the display (CopyFromParent means we just ask the screen itself instead of specifying)
                              InputOutput, //We want to be able to interact with the window, so it must be InputOutput and Not InputOnly
                              CopyFromParent, //What data the pixels expect (CopyFromParent means we just ask the screen itself instead of specifying) 
                              CWBackPixel|CWBorderPixel, //We only change the background_pixel and border_pixel
                              &m_windowAttributes); //Hand over the values for the 2 variables above
    XMapWindow(m_xDisplay,m_xWindow);
    XMoveWindow(m_xDisplay,m_xWindow,m_displaySize("width")/2,m_displaySize("height")/2);
    XFlush(m_xDisplay);
    #endif
    m_StartLoop();
    
}

void RPEngine::m_StartLoop()
{
  m_bRunning = true;
  
  if(m_mode == EngineMode::FunctionMode)
  {
      m_funcInitPtr();
      
      while(m_bRunning)
      {
          //Do events n stuff
          m_funcUpdatePtr(); //User Update
      }
      
  }
  else
  {
      Init();
      
      while(m_bRunning)
      {
          
          Run();
      }
  }
  
}

void RPEngine::Stop()
{
    m_bRunning = false;
}

void RPEngine::DrawLine(Point2D point1, Point2D point2)
{
    #ifdef X11
    XDrawLine(m_xDisplay, m_xWindow, m_context, point1.x, point1.y, point2.x, point2.y);
    #endif
}
void RPEngine::DrawLine(float x1, float y1, float x2, float y2)
{
    
    #ifdef X11
    XDrawLine(m_xDisplay, m_xWindow, m_context, x1, y1, x2, y2);
    #endif
}

void RPEngine::DrawShape(Point2D points[], int length)
{
    Point2D prevPoint = Point2D(0,0);
    for(int i = 0; i < length;i++)
    {
        if(i == 0)
        {
            prevPoint = points[i];
        }

        else if(i == length-1)
        {
            DrawLine(prevPoint, points[i]);
            prevPoint = points[i];
            DrawLine(prevPoint, points[0]);
        }
        else
        {
            DrawLine(prevPoint, points[i]);
            prevPoint = points[i];
        }
        
    }
}

void RPEngine::Fill(Color c)
{
   m_colorInUse = Color::ColorCopy(c);
}

void RPEngine::Fill(uint8_t R, uint8_t G, uint8_t B)
{
    m_colorInUse = new Color(R,G,B);
}




