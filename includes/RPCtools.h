#pragma once
#include <iostream>
struct CTools
{
  template <typename T>
  static int compareArray(T a[],T b[],int size)
  {
    for(int i=0;i<size;i++)
    {
        if(a[i]!=b[i])
        return 0;
    }
    return 1;
}
  
};
