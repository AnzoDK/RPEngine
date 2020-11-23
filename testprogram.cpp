#include "RPEngine.h"

class Game : public RPEngine
{
    public:
        Game() : RPEngine(480,480)
        {
            
        }
        
        void Init() override
        {
            std::cout << "Starting Engine" << std::endl;
        }
        
        void Run() override
        {
            if(m_iFrameCounter%100 == 0)
            {
                std::cout << "Frame " << m_iFrameCounter << std::endl;
            }
            //Fill(Color(255,0,0));
            //DrawLine(0,0,480,480);
            Point2D points[6] = {Point2D(0,0),Point2D(60,80),Point2D(100,60),Point2D(200,40),Point2D(400,0),Point2D(500,34)};
            DrawShape(points,6);
            m_iFrameCounter++;
            
        }
    private:
        unsigned long int m_iFrameCounter;
};



int main()
{
    
  Game* game = new Game();
  game->Start();
  delete(game);
  return 0;  
}
