#pragma once
#include <iostream>
struct CTools
{
  template <typename T>
  static bool compareArray(T a[],T b[],int size)
  {
    for(int i=0;i<size;i++)
    {
        std::cout << "Comparing: ";
        std::cout << std::hex << (int)a[i];
        std::cout << " With ";
        std::cout << std::hex << (int)b[i] << std::endl;
        if(a[i]!=b[i])
        return 0;
    }
    return 1;
}
  
};
