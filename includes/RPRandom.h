#pragma once
#include <iostream>
#include <random>
#include <time.h>
#include <cstdlib>
namespace rp
{
    class Random
    {
        public:
            Random(){}
            static float GenerateF(float min, float max)
            {
                std::random_device dev;
                std::mt19937 rng(dev());
                std::uniform_real_distribution<> dist6(min,max);

                return dist6(rng);
                
            }
            
            static int GenerateI(int min, int max)
            {
                std::random_device dev;
                std::mt19937 rng(dev());
                std::uniform_int_distribution<std::mt19937::result_type> dist6(min,max);

                return dist6(rng);
                
            }
        private:
            
    };
}
