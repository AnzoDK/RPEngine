#include <iostream>
#include "includes/RPEngine.h"
int main(int argc, char **argv)
{
    rp::RosenoernAudio* ra = new rp::RosenoernAudio(1,10);
    ra->init();
    ra->AddToQueue("sound.ogg");
    std::cout << "Hello, world!" << std::endl;
    ra->PlayFromQueue();
    usleep(5000000);
    delete(ra);
    return 0;
}
