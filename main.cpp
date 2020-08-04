#include <iostream>
#include <future>
#include <chrono>
#include "includes/RPEngine.h"
/*
std::string readCin()
{
    std::string line;
    std::getline(std::cin, line);
    return line;
}
int main(int argc, char **argv)
{
    rp::RosenoernAudio* ra = new rp::RosenoernAudio(0,10);
    std::cout << "Hello, world!" << std::endl;
    ra->init();
    ra->AddToQueue("sound.ogg");
    ra->PlayFromQueue();
    bool run = 1;
    bool playing = 0;
    
    std::string input = "";
    auto future = std::async(std::launch::async,readCin);
    while(run)
    {
     if(future.wait_for(std::chrono::milliseconds(5)) == std::future_status::ready)
     {
         input = future.get();
         future = std::async(std::launch::async,readCin);
         
     }
     if(input == "exit")
     {
       run = 0;
     }
     else if(input != "")
     {
       ra->AddToQueue(input);
     }
     if(!playing && (ra->GetQueueLength() != 0))
     {
         playing = 1;
         ra->PlayFromQueue();
     }
     input = "";
    }
    delete(ra);
    std::cout << "Cleaning Complete" << std::endl;
    usleep(5000000);
    delete(ra);
    return 0;
}
*/
int main()
{
    rp::RosenoernEngine* re = new rp::RosenoernEngine(1,10);
    re->init();
    rp::RosenoernAudio& audioController = re->GetAudioController();
    audioController.AddToQueue("sound.ogg");
    audioController.PlayFromQueue();
    std::string input;
    std::cin >> input;
    delete(re);
    return 0;
}
