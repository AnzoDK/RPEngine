#pragma once
#include <iostream>
#include <filesystem>
namespace fs = std::filesystem;

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


class RPEngine
{
    public:
        RPEngine(int width, int height);
        ~RPEngine();
    private:
        DoubleItem<int, int> m_screenSize;
};
