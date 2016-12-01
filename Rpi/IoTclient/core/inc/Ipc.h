#ifndef IPC_H
#define IPC_H
#include "Poco/Thread.h"
#include "Poco/Runnable.h"
#include "mqtt/async_client.h"

namespace execConn
{

using namespace Poco;
using namespace std;


class Ipc : public Runnable
{
public:

    Ipc(int k = -1) : Runnable(), n(k) {}

    virtual void run();


private:
    int n;

};

}

#endif // IPC_H
