#include <thread>
#include <chrono>
#include "mqtt/async_client.h"
#include "PublishingService.h"
#include "Ipc.h"
#include "Util.h"


namespace execConn
{

const std::string ADDR("LoRa");
const std::string DEV_SERIAL ("1116GD00E0FDGOAZ");
const std::string DEV_PASSWORD ("di0j6t1hq6cs7r0m");

const std::string IPC_ADDR ("tcp://localhost:1883");
const std::string IPC_CLI ("ipcCli");

const std::string TOPIC("ipc/LoRa");

const int  QOS = 1;
const long TIMEOUT = 10000L;

/////////////////////////////////////////////////////////////////////////////

class action_listener : public virtual mqtt::iaction_listener
{
    std::string name_;

    virtual void on_failure(const mqtt::itoken& tok) {
        std::cout << name_ << " failure";
        if (tok.get_message_id() != 0)
            std::cout << " (token: " << tok.get_message_id() << ")" << std::endl;
        std::cout << std::endl;
    }

    virtual void on_success(const mqtt::itoken& tok) {
        std::cout << name_ << " success";
        if (tok.get_message_id() != 0)
            std::cout << " (token: " << tok.get_message_id() << ")" << std::endl;
        if (!tok.get_topics().empty())
            std::cout << "\ttoken topic: '" << tok.get_topics()[0] << "', ..." << std::endl;
        std::cout << std::endl;
    }

public:
    action_listener(const std::string& name) : name_(name) {}
};

void taskMqtt(string msg);

class callback : public virtual mqtt::callback,
        public virtual mqtt::iaction_listener

{
    int nretry_;
    mqtt::async_client& cli_;
    action_listener& listener_;

    void reconnect() {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        mqtt::connect_options connOpts;
        connOpts.set_keep_alive_interval(20);
        connOpts.set_clean_session(true);

        try {
            cli_.connect(connOpts, nullptr, *this);
        }
        catch (const mqtt::exception& exc) {
            std::cerr << "Error: " << exc.what() << std::endl;
            exit(1);
        }
    }

    // Re-connection failure
    virtual void on_failure(const mqtt::itoken& tok) {
        std::cout << "Reconnection failed." << std::endl;
        if (++nretry_ > 5)
            exit(1);
        reconnect();
    }

    // Re-connection success
    virtual void on_success(const mqtt::itoken& tok) {
        std::cout << "Reconnection success" << std::endl;;
        cli_.subscribe(TOPIC, QOS, nullptr, listener_);
    }

    virtual void connection_lost(const std::string& cause) {
        std::cout << "\nConnection lost" << std::endl;
        if (!cause.empty())
            std::cout << "\tcause: " << cause << std::endl;

        std::cout << "Reconnecting." << std::endl;
        nretry_ = 0;
        reconnect();
    }

    virtual void message_arrived(const std::string& topic, mqtt::message_ptr msg) {
        std::cout << "Message arrived" << std::endl;
        std::cout << "\ttopic: '" << topic << "'" << std::endl;
        std::cout << "\t'" << msg->to_str() << "'\n" << std::endl;

        thread t1(taskMqtt, msg->to_str());
        t1.detach();

    }

    virtual void delivery_complete(mqtt::idelivery_token_ptr token) {}

public:
    callback(mqtt::async_client& cli, action_listener& listener)
        : cli_(cli), listener_(listener) {}
};


void taskMqtt(string msg)
{
    try {

        const std::string mqttMessage = Util::loRaToMqtt(msg);

        wolkabout::Device dev(DEV_SERIAL,DEV_PASSWORD);

        wolkabout::PublishingService test(dev);

        test.publish(mqttMessage);

        //sleep(2);


    }
    catch (const mqtt::exception& exc) {
        std::cerr << "Error: " << exc.what() << std::endl;
        return ;
    }
}




void Ipc::run()
{


    mqtt::async_client client(IPC_ADDR, IPC_CLI);
    action_listener subListener("Subscription");

    callback cb(client, subListener);
    client.set_callback(cb);

    mqtt::connect_options connOpts;
    connOpts.set_keep_alive_interval(20);
    connOpts.set_clean_session(true);

    try {
        mqtt::itoken_ptr conntok = client.connect(connOpts);
        std::cout << "Waiting for the connection..." << std::flush;
        conntok->wait_for_completion();
        std::cout << "OK" << std::endl;



        client.subscribe(TOPIC, QOS, nullptr, subListener);





        while (std::tolower(std::cin.get()) != 'q')
            ;

        std::cout << "Disconnecting..." << std::flush;
        conntok = client.disconnect();
        conntok->wait_for_completion();
        std::cout << "OK" << std::endl;
    }
    catch (const mqtt::exception& exc) {
        std::cerr << "Error: " << exc.what() << std::endl;
        return ;
    }

}




}
