#include "RPEngine.h"

class Player : public Base2D
{
    public:
        Player() : Base2D()
        {
            
        }
        Player(Point2D spawnPoint) : Base2D(spawnPoint)
        {
            
        }
        virtual void Update() override {m_position.x+=0.0; m_playerRect = Rect(m_position.x,m_position.y,m_playerRect.w,m_playerRect.h);};
        virtual void Draw(RPEngine& renderEngine) override {renderEngine.DrawShape(m_playerRect.ToShape());};
    protected:
        Rect m_playerRect = Rect(0,0,100,100);
};
class Game : public RPEngine
{

    public:
        Game() : RPEngine(480,480)
        {
            
        }
        
        void UserInit() override
        {
            std::cout << "Starting Engine" << std::endl;
            m_player = Player({100,100});
        }
        
        void UserLoop() override //loop
        {
            //ClearScreen();
            if(m_iFrameCounter%100 == 0)
            {
                std::cout << "Frame " << m_iFrameCounter << std::endl;
            }
            //Fill(Color(255,0,0));
            //DrawLine(0,0,480,480);
            
            //DrawShape(strangeShape);
            m_player.Update();
            m_player.Draw(*this);
            m_iFrameCounter++;
            //std::cout << "Frame took: " << GetDeltaTime() << " seconds to execute" << std::endl;
            
            
        }
         
    private:
        Shape strangeShape = Shape({Point2D(0,0),Point2D(60,80),Point2D(100,60),Point2D(200,40),Point2D(400,0),Point2D(500,34)});
        Player m_player;
        unsigned long int m_iFrameCounter;
};



int main()
{
    
  Game* game = new Game();
  game->Start();
  delete(game);
  return 0;  
}
