#include <iostream>
#include <chrono>
#include <vector>
#include "Util.h"

namespace
{
const int WOLKSENSE_MULTIPLIER_FACTOR = 10;
const int PRESSURE_CONVERSION_FACTOR = 10 * WOLKSENSE_MULTIPLIER_FACTOR;
}

using namespace Poco::JSON;
using namespace std::chrono;

namespace execConn
{

const std::string Util::LORA_TEMPERATURE= "Temp";
const std::string Util::LORA_HUMIDITY= "Humidity";
const std::string Util::LORA_BAROMETER = "Barometer";
const std::string Util::LORA_POLLUTION = "Pollution";
const std::string Util::LORA_ADDRESS= "Address";

const std::string Util::MQTT_TEMPERATURE = "T";
const std::string Util::MQTT_HUMIDITY = "H";
const std::string Util::MQTT_BAROMETER = "P";
const std::string Util::MQTT_GENERIC = "GEN";

const std::string Util::MQTT_RTC = "RTC";
const std::string Util::MQTT_READINGS = "READINGS";
const std::string Util::MQTT_TIMESTAMP = "R";

const int Util::WOLKSENSE_TEMP_MIN_VALUE = -400;
const int Util::WOLKSENSE_TEMP_MAX_VALUE = 850;
const int Util::WOLKSENSE_PRESSURE_MIN_VALUE = 8000;
const int Util::WOLKSENSE_PRESSURE_MAX_VALUE = 11000;
const int Util::WOLKSENSE_HUMID_MIN_VALUE = 10;
const int Util::WOLKSENSE_HUMID_MAX_VALUE = 1000;

const int Util::WOLKSENSE_GENERIC_MIN_VALUE = 0;
const int Util::WOLKSENSE_GENERIC_MAX_VALUE = 1000;

namespace
{
const int WOLKSENSE_MULTIPLIER_FACTOR = 10;
const int PRESSURE_CONVERSION_FACTOR = 10;
}

using namespace Poco::JSON;
using namespace std::chrono;

std::string Util::loRaToMqtt(const std::string& json)
{

    Parser loParser;

    Poco::Dynamic::Var loParsedJson = loParser.parse(json.c_str());
    Poco::Dynamic::Var loParsedJsonResult = loParser.result();

    Object::Ptr loJsonObject = loParsedJsonResult.extract<Object::Ptr>();

    std::string temp = getValue(loJsonObject, LORA_TEMPERATURE);
    std::string humid = getValue(loJsonObject, LORA_HUMIDITY);
    std::string pressure = getValue(loJsonObject, LORA_BAROMETER);
    std::string pollution = getValue(loJsonObject, LORA_POLLUTION);

    try
    {
        float p = std::stof(pressure);

        pressure = floatToWolksenseStrInt(p, WOLKSENSE_MULTIPLIER_FACTOR, true,
                                          WOLKSENSE_PRESSURE_MIN_VALUE, WOLKSENSE_PRESSURE_MAX_VALUE);
    }
    catch(...)
    {
        std::cout<<"Could not parse pressure value: " << pressure<<std::endl;
    }

    try
    {
        temp = strFloatToWolksenseStrInt(temp, WOLKSENSE_MULTIPLIER_FACTOR, true,
                                         WOLKSENSE_TEMP_MIN_VALUE, WOLKSENSE_TEMP_MAX_VALUE);
    }
    catch(...)
    {
        std::cout<<"Could not parse temperature value: "<< temp <<std::endl;
    }

    try
    {
        humid = strFloatToWolksenseStrInt(humid, WOLKSENSE_MULTIPLIER_FACTOR, true,
                                          WOLKSENSE_HUMID_MIN_VALUE, WOLKSENSE_HUMID_MAX_VALUE);
    }
    catch(...)
    {
        std::cout<<"Could not parse humidity value: " <<  humid <<std::endl;
    }

    try
    {
        pollution = strFloatToWolksenseStrInt(pollution, WOLKSENSE_MULTIPLIER_FACTOR, true,
                                          WOLKSENSE_GENERIC_MIN_VALUE, WOLKSENSE_GENERIC_MAX_VALUE);
    }
    catch(...)
    {
        std::cout<< "Could not parse light value: " << pollution <<std::endl;
    }


    seconds sec = duration_cast<seconds>(system_clock::now().time_since_epoch());
    unsigned timestamp = sec.count();

    std::stringstream mqttMsg;
    mqttMsg << MQTT_RTC << " " << timestamp << ";";
    mqttMsg << MQTT_READINGS << " ";
    mqttMsg << MQTT_TIMESTAMP << ":" << timestamp << ",";
    if(!temp.empty())
    {
        mqttMsg << MQTT_TEMPERATURE << ":" << temp << ",";
    }
    if(!humid.empty())
    {
        mqttMsg << MQTT_HUMIDITY << ":" << humid << ",";
    }
    if(!pressure.empty())
    {
        mqttMsg << MQTT_BAROMETER << ":" << pressure << ",";
    }
    if(!pollution.empty())
    {
        mqttMsg << MQTT_GENERIC << ":" << pollution << ",";
    }


    std::string msgStr = mqttMsg.str();
    if(msgStr.back() == ',')
    {
        msgStr.pop_back();
    }
    msgStr.append(";");

    return msgStr;

}


std::string Util::getJSONValue(const std::string& json, const std::string& key)
{
    Parser loParser;

    Poco::Dynamic::Var loParsedJson = loParser.parse(json.c_str());
    Poco::Dynamic::Var loParsedJsonResult = loParser.result();

    Object::Ptr loJsonObject = loParsedJsonResult.extract<Object::Ptr>();

    return getValue(loJsonObject, key.c_str());
}


std::string Util::getValue(Poco::JSON::Object::Ptr aoJsonObject, const std::string& key)
{
    Poco::Dynamic::Var loVariable;
    std::string lsReturn;

    loVariable = aoJsonObject->get(key);

    if(!loVariable.isEmpty())
    {
        lsReturn = loVariable.convert<std::string>();
    }

    return lsReturn;
}




std::string Util::strFloatToWolksenseStrInt(const std::string& floatValue, int multiplier, bool limit, int min, int max)
{
    std::stringstream wsv;
    float f = std::stof(floatValue);
    f *= multiplier;
    int intVal = static_cast<int>(f);
    if(limit)
        intVal = limitInt(intVal, min, max);
    unsigned absIntVal = std::abs(intVal);

    if(intVal >= 0)
        wsv << "+";
    else
        wsv << "-";

    if(absIntVal < 10)
        wsv << "0";
    wsv << absIntVal;

    return wsv.str();
}

std::string Util::floatToWolksenseStrInt(float value, int multiplier, bool limit, int min, int max)
{
    return strFloatToWolksenseStrInt(std::to_string(value), multiplier, limit, min, max);
}

int Util::limitInt(int value, int min, int max)
{
    int limitedValue;
    if(value < min)
        limitedValue = min;
    else if(value > max)
        limitedValue = max;
    else
        limitedValue = value;

    return limitedValue;
}

}
