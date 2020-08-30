#pragma once
#include <iostream>
#include "RPUI.h"
namespace rp
{
    struct CppTools
    {
    static void BaseVectorSort(std::vector<Base*>& vec)
    {
        //This is essentially just a bubble sort
        bool sorted = 0;
        int counter = 0;
        short int changes = 0;
        Base* tmpBase = nullptr;
        while(!sorted)
        {
            if(counter == vec.size()-1)
            {
                counter = 0;
                if(changes > 0)
                {
                    changes = 0;
                }
                else
                {
                    sorted = 1;
                }
            }
            if((vec.at(counter)->GetZ() > vec.at(counter+1)->GetZ()) && !sorted)
            {
                tmpBase = vec.at(counter);
                vec.at(counter) = vec.at(counter+1);
                vec.at(counter+1) = tmpBase;
                changes+=1;
            }
            counter++;
        }
        delete(tmpBase); /*<--- As this is never declared as a new varaible, it could couse issues if we delete it*/
    }
    };
}
