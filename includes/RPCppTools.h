#pragma once
#include <iostream>
#include "RPUI.h"
namespace rp
{
    struct CppTools
    {
        //I HAD TO MAKE A BAD BUBBLE SORT JUST TO FIX STD::SORT TELLING ME THAT -1 > 1.... WHY! WHAT THE FUCK!? - so if you want to improve this function - feel free to do so :)
    static void BaseVectorSort(std::vector<Base*>& vec)
    {
        //This is essentially just a bubble sort
        bool sorted = 0;
        unsigned int counter = 0;
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
        //delete(tmpBase); /*<--- As this is never declared as a new varaible, it could couse issues if we delete it*/
    }
    };
}
