#pragma once
#include <iostream>
struct CTools
{
  template <typename T>
  static bool compareArray(T a[],T b[],int size)
  {
    for(int i=0;i<size;i++)
    {
        std::cout << "Comparing: " << a[i] << "With " << b[i] << std::endl;
        if(a[i]!=b[i])
        return 0;
    }
    return 1;
}
  
};
