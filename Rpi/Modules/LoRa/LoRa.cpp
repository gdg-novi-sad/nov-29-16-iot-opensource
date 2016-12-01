#include "Poco/Thread.h"
#include "Poco/Runnable.h"
#include "LoRaEngine.h"

int main()
{

    execLoRa::LoraBus w;
    Poco::Thread t;
    w.LoraInit("/dev/ttyAMA0");
    t.start(w);

    while (std::tolower(std::cin.get()) != 'q')
        ;

    return 0;
}
