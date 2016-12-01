#include <Poco/StringTokenizer.h>
#include <Poco/JSON/Object.h>
#include <iostream>
#include <unistd.h>
#include "LoRaEngine.h"
#include "mqtt/async_client.h"

namespace execLoRa
{

const std::string ADDRESS("tcp://localhost:1883");
const std::string CLIENTID("AsyncPublisher");
const std::string TOPIC("ipc/LoRa");

const int  QOS = 1;
const long TIMEOUT = 10000L;


static vector<string> split(const char *str, char c = ' ')
{
    vector<string> result;

    do
    {
        const char *begin = str;

        while(*str != c && *str)
            str++;

        result.push_back(string(begin, str));
    } while (0 != *str++);

    return result;
}

void printArray(Poco::JSON::Array::Ptr array);


void printObject(Poco::JSON::Object::Ptr object){
    //print the object
    Poco::JSON::Object::ConstIterator it = object->begin();
    Poco::JSON::Object::ConstIterator endit = object->end();
    while(it != endit){
        std::cout << it->first << " : ";
        if(it->second.isNumeric()){
            int val=0;
            it->second.convert<int>(val);
            std::cout << val;
        }else if(it->second.isString()){
            std::string val;
            it->second.convert<std::string>(val);
            std::cout << val;
        }else if(it->second.isBoolean()){
            bool val=0;
            it->second.convert<bool>(val);
            std::cout << val;
        }else if(it->second.isEmpty()){
            std::cout << "null";
        }else {
            //array
            try{
                Poco::JSON::Array::Ptr spArray;
                spArray = it->second.extract<Poco::JSON::Array::Ptr>();
                printArray(spArray);
                ++it;
                continue;
            }
            catch(Poco::BadCastException&){/*continue silently*/}
            //object
            try{
                Poco::JSON::Object::Ptr spObject;
                spObject = it->second.extract<Poco::JSON::Object::Ptr>();
                printObject(spObject);
                ++it;
                continue;
            }
            catch(Poco::BadCastException&){/*continue silently*/}
        }
        std::cout << std::endl;
        ++it;
    }
}

void printArray(Poco::JSON::Array::Ptr array){
    for(unsigned int i=0; i<array->size(); ++i){
        Poco::JSON::Object::Ptr spObj = array->getObject(i);
        //print object
        printObject(spObj);
    }
}


/**
 * A callback class for use with the main MQTT client.
 */
class callback : public virtual mqtt::callback
{
public:
    virtual void connection_lost(const std::string& cause) {
        std::cout << "\nConnection lost" << std::endl;
        if (!cause.empty())
            std::cout << "\tcause: " << cause << std::endl;
    }

    // We're not subscribed to anything, so this should never be called.
    virtual void message_arrived(const std::string& topic, mqtt::message_ptr msg) {}

    virtual void delivery_complete(mqtt::idelivery_token_ptr tok) {
        std::cout << "Delivery complete for token: "
                  << (tok ? tok->get_message_id() : -1) << std::endl;
    }
};

/**
 * A base action listener.
 */
class action_listener : public virtual mqtt::iaction_listener
{
protected:
    virtual void on_failure(const mqtt::itoken& tok) {
        std::cout << "\n\tListener: Failure on token: "
                  << tok.get_message_id() << std::endl;
    }

    virtual void on_success(const mqtt::itoken& tok) {
        std::cout << "\n\tListener: Success on token: "
                  << tok.get_message_id() << std::endl;
    }
};

class delivery_action_listener : public action_listener
{
    bool done_;

    virtual void on_failure(const mqtt::itoken& tok) {
        action_listener::on_failure(tok);
        done_ = true;
    }

    virtual void on_success(const mqtt::itoken& tok) {
        action_listener::on_success(tok);
        done_ = true;
    }

public:
    delivery_action_listener() : done_(false) {}
    bool is_done() const { return done_; }
};


int sendMessage(const std::string message)
{
    mqtt::async_client client(ADDRESS, CLIENTID);

    callback cb;
    client.set_callback(cb);

    try {
        mqtt::itoken_ptr conntok = client.connect();
        std::cout << "Waiting for the connection..." << std::flush;
        conntok->wait_for_completion();
        std::cout << "OK" << std::endl;


        // Now try with itemized publish.

        std::cout << "Sending next message..." << std::flush;
        mqtt::idelivery_token_ptr pubtok;
        pubtok = client.publish(TOPIC, message.c_str(), std::strlen(message.c_str()), QOS, false);
        pubtok->wait_for_completion(TIMEOUT);
        std::cout << "OK" << std::endl;


        // Disconnect
        std::cout << "Disconnecting..." << std::flush;
        conntok = client.disconnect();
        conntok->wait_for_completion();
        std::cout << "OK" << std::endl;
    }
    catch (const mqtt::exception& exc) {
        std::cerr << "Error: " << exc.what() << std::endl;
        return 1;
    }

    return 0;
}

void LoraBus::run()
{
    while (1)
    {
        sp.write(RECEIVE_LORA);
        sleep(3);
        rsp="";
        /*Assigning random value to c*/
        char c='A';

        while(c!='\n')
        {
            c = sp.read();
            rsp+=c;
        }



        std::cout<<"Receive msgh "<<rsp<<std::endl;

        rsp="";

        /*Assigning random value to c*/
        c='A';

        while(c!='\n')
        {
            c = sp.read();
            rsp+=c;
        }
        std::cout<<"Message received: "<<rsp<<std::endl;

        std::vector<string> tokens = split(rsp.c_str(), ' ');



        //TODO: No hardcoded values!
        std::vector<string> tst = split(tokens[2].c_str(), 'F');
        tst[0].erase(0, 1);

        for (unsigned int i=0; i<tst.size();i++){
            std::replace( tst[i].begin(), tst[i].end(), 'C', '.'); // replace all 'C' to '.' Unqable to send '.' in LoRa message
            std::cout << tst[i] <<std::endl;
        }

        Poco::JSON::Object::Ptr result = new Poco::JSON::Object;
        result->set("Temp", tst[2]);
        result->set("Humidity", tst[1]);
        result->set("Barometer", tst[0]);
        result->set("Pollution", tst[3]);
        result->set("Address", "LoRa");


        stringstream ss;
        result->stringify(ss);
        auto jsonData = ss.str();
        std::cout << jsonData <<std::endl;

        sendMessage(jsonData);
    }
}


void LoraBus::LoraInit (std::string port)
{
    sp.open(port, 57600);
    sp.write(RESET_LORA);
    sleep(3);
    sp.read(rsp);
    std::cout<<"Reset command response:"<<rsp<<std::endl;
    sp.write(WATCHDOG_LORA);
    sleep(3);
    sp.read(rsp);
    std::cout<<"Watchdog command response:"<<rsp<<std::endl;
    sp.write(MAC_PAUSE_LORA);
    sleep(3);
    sp.read(rsp);;
    std::cout<<"Mac pause response:"<<rsp<<std::endl;
}







}
