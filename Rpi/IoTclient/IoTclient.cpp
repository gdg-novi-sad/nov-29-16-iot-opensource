#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "Poco/Thread.h"
#include "Poco/Runnable.h"
#include "Ipc.h"

#include "Poco/AsyncChannel.h"
#include "Poco/SplitterChannel.h"
#include "Poco/FileChannel.h"
#include "Poco/ConsoleChannel.h"
#include "Poco/AutoPtr.h"
#include "Poco/FormattingChannel.h"
#include "Poco/PatternFormatter.h"

#include "WebClient.h"


void RegisterSensors (const std::string name, const std::string username, const std::string password)
{
    wolkabout::Device device = wolkabout::WebClient::activateDeviceForUser(username, password, name);

    const std::string obtainedSerial = device.getSerial();
    const std::string obtainedPassword = device.getPassword();

    std::cout << "Device " << name << "obtained following serial and password:" << std::endl;
    std::cout << "Username: " << obtainedSerial << std::endl;
    std::cout << "Password: " << obtainedPassword << std::endl;

}

int main(int argc, char* argv[])
{

    if (argc==4)
    {
        RegisterSensors (argv[1], argv[2], argv[3]);
    } else
    {
        execConn::Ipc comm;
        Poco::Thread t;
        t.start(comm);
        t.join();
    }

    return 0;
}
