#ifndef WOLKSENSEUTIL_H
#define WOLKSENSEUTIL_H

#include <Poco/JSON/JSON.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>
#include <string>

namespace execConn
{

class Util
{
public:
    static std::string loRaToMqtt(const std::string& json);
    static std::string getJSONValue(const std::string& json, const std::string& key);

    static const std::string LORA_TEMPERATURE;
    static const std::string LORA_HUMIDITY;
    static const std::string LORA_BAROMETER;
    static const std::string LORA_POLLUTION;
    static const std::string LORA_ADDRESS;

    static const std::string MQTT_TEMPERATURE;
    static const std::string MQTT_HUMIDITY;
    static const std::string MQTT_BAROMETER;
    static const std::string MQTT_ADDRESS;
    static const std::string MQTT_RTC;
    static const std::string MQTT_READINGS;
    static const std::string MQTT_TIMESTAMP;
    static const std::string MQTT_GENERIC;

    static const int WOLKSENSE_TEMP_MIN_VALUE;
    static const int WOLKSENSE_TEMP_MAX_VALUE;
    static const int WOLKSENSE_PRESSURE_MIN_VALUE;
    static const int WOLKSENSE_PRESSURE_MAX_VALUE;
    static const int WOLKSENSE_HUMID_MIN_VALUE;
    static const int WOLKSENSE_HUMID_MAX_VALUE;
    static const int WOLKSENSE_GENERIC_MIN_VALUE;
    static const int WOLKSENSE_GENERIC_MAX_VALUE;



private:
    static std::string getValue(Poco::JSON::Object::Ptr aoJsonObject, const std::string& key);

    static std::string strFloatToWolksenseStrInt(const std::string& floatValue, int multiplier = 1,
                                                 bool limit = false, int min = 0, int max = 0);
    static std::string floatToWolksenseStrInt(float value, int multiplier = 1, bool limit = false,
                                              int min = 0, int max = 0);
    static int limitInt(int value, int min, int max);
};

}

#endif // WOLKSENSEUTIL_H
