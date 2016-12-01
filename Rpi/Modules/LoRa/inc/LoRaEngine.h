#ifndef LORA_BUS_H
#define LORA_BUS_H

#include "Poco/Serial/SerialPort.h"
#include "Poco/Thread.h"
#include "Poco/Runnable.h"
#include <iostream>


namespace execLoRa
{


using namespace Poco;
using namespace std;

class LoraBus : public Runnable
{
public:
    LoraBus(int k = -1) : Runnable(), n(k) {}

    virtual void run();

    void LoraInit (std::string port);

private:
    static constexpr const char* RESET_LORA = "sys reset\r\n";
    static constexpr const char* WATCHDOG_LORA = "radio set wdt 0\r\n";
    static constexpr const char* MAC_PAUSE_LORA = "mac pause\r\n";
    static constexpr const char* RECEIVE_LORA = "radio rx 0\r\n";
    int n;
    Poco::Serial::SerialPort sp;
    std::string rsp;
};



}

#endif // LORA_BUS_H
