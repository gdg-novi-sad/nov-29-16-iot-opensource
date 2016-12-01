/*
 * Copyright 2016 WolkAbout Technology s.r.o.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * Modifications copyright (C) 2013 <Execom/misavic>
 */

#include "WebClient.h"

#include "Device.h"
#include "WebClientException.h"
#include <Poco/JSON/JSON.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>
#include "Util.h"

#include <Poco/Net/HTTPSClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/StreamCopier.h>
#include <Poco/Path.h>
#include <Poco/URI.h>
#include <Poco/Exception.h>


#include <cstdio>

using namespace Poco::JSON;
using namespace Poco::Net;
using namespace Poco;

namespace wolkabout
{
Device WebClient::activateDeviceForUser(const std::string& userEmail, const std::string& userPassword,
                                        const std::string& deviceName)
{
    auto accessToken = getAccessToken(userEmail, userPassword);

    auto deviceSerial = getRandomSerial(accessToken, DeviceType::GENERIC);

    return activateDevice(accessToken, deviceSerial, deviceName);
}

std::string WebClient::getAccessToken(const std::string& userEmail, const std::string& userPassword)
{
    Poco::URI uri(SIGN_IN);
    const Poco::Net::Context::Ptr context( new Poco::Net::Context( Poco::Net::Context::CLIENT_USE, "", "", "",Poco::Net::Context::VERIFY_NONE ) );
    Poco::Net::HTTPSClientSession session(uri.getHost(),uri.getPort(), context);


    HTTPRequest req(HTTPRequest::HTTP_POST, uri.getPath(), HTTPMessage::HTTP_1_1);
    req.setContentType("application/json");

    const int jsonMaxBodySize = 200;
    char body[jsonMaxBodySize];

    std::snprintf(body, jsonMaxBodySize, R"({"email":"%s","password":"%s"})", userEmail.c_str(), userPassword.c_str());


    std::string reqBody(body);
    req.setContentLength(reqBody.length() );
    try
    {
    std::ostream& myOStream = session.sendRequest(req); // sends request, returns open stream
    myOStream << reqBody;  // sends the body
    } catch (Poco::Exception& error) {
        std::cout << "Error : " << error.displayText() << std::endl;
    }


    Poco::Net::HTTPResponse res;
    std::istream& iStr = session.receiveResponse(res);  // get the response from server
    std::string content{ std::istreambuf_iterator<char>(iStr),
                         std::istreambuf_iterator<char>() };


    if (res.getStatus() == Poco::Net::HTTPResponse::HTTP_UNAUTHORIZED)
    {
        throw WebClientException("Invalid credentials\n");
    } else if (res.getStatus() != Poco::Net::HTTPResponse::HTTP_OK)
    {
        throw WebClientException("Error authenticating user\n");
    }

    return execConn::Util::getJSONValue(content, "accessToken");

}

std::string WebClient::getRandomSerial(const std::string& accessToken, const DeviceType& deviceType)
{
    std::string addr = std::string(RANDOM_SER)  + getDeviceTypeString(deviceType) ;
    Poco::URI uri(addr);
    const Poco::Net::Context::Ptr context( new Poco::Net::Context( Poco::Net::Context::CLIENT_USE, "", "", "",Poco::Net::Context::VERIFY_NONE ) );
    Poco::Net::HTTPSClientSession session(uri.getHost(),uri.getPort(), context);


    HTTPRequest req(HTTPRequest::HTTP_GET, uri.getPath(), HTTPMessage::HTTP_1_1);

    //Headers
    req.setContentType("application/json");
    req.set("Cache-Control", "no-cache");
    if (!accessToken.empty())
    {
        req.set("Authorization", accessToken);
    }

    //Send request
    try
    {
        session.sendRequest(req); // sends request, returns open stream
    } catch (Poco::Exception& error) {
        std::cout << "Error : " << error.displayText() << std::endl;
    }


    Poco::Net::HTTPResponse res;
    std::istream& iStr = session.receiveResponse(res);  // get the response from server
    std::string content{ std::istreambuf_iterator<char>(iStr),
                         std::istreambuf_iterator<char>() };


    if (res.getStatus() == Poco::Net::HTTPResponse::HTTP_UNAUTHORIZED)
    {
        throw WebClientException("Invalid credentials\n");

    } else if (res.getStatus() != Poco::Net::HTTPResponse::HTTP_OK)
    {
        throw WebClientException("Error authenticating user\n");
    }


    return  execConn::Util::getJSONValue(content, "serial");
}

Device WebClient::activateDevice(const std::string& accessToken, const std::string& deviceSerial,
                                 const std::string& deviceName)
{
    std::string addr = std::string(ACTIVATE_DEV) + deviceSerial ;
    Poco::URI uri(addr);
    const Poco::Net::Context::Ptr context( new Poco::Net::Context( Poco::Net::Context::CLIENT_USE, "", "", "",Poco::Net::Context::VERIFY_NONE ) );
    Poco::Net::HTTPSClientSession session(uri.getHost(),uri.getPort(), context);


    HTTPRequest req(HTTPRequest::HTTP_POST, uri.getPath(), HTTPMessage::HTTP_1_1);

    //Headers
    req.setContentType("application/json");
    req.set("Cache-Control", "no-cache");
    if (!accessToken.empty())
    {
        req.set("Authorization", accessToken);
    }


    //Body
    const int jsonMaxBodySize = 200;
    char body[jsonMaxBodySize];

    std::snprintf(body, jsonMaxBodySize, R"({"name":"%s"})", deviceName.c_str());
    std::string reqBody(body);
    req.setContentLength(reqBody.length() );

    //Send request
    try
    {
    std::ostream& myOStream = session.sendRequest(req); // sends request, returns open stream
    myOStream << reqBody;  // sends the body
    } catch (Poco::Exception& error) {
        std::cout << "Error : " << error.displayText() << std::endl;
    }


    Poco::Net::HTTPResponse res;
    std::istream& iStr = session.receiveResponse(res);  // get the response from server
    std::string content{ std::istreambuf_iterator<char>(iStr),
                         std::istreambuf_iterator<char>() };


    if (res.getStatus() == Poco::Net::HTTPResponse::HTTP_UNAUTHORIZED)
    {
        throw WebClientException("Invalid credentials\n");

    } else if (res.getStatus() != Poco::Net::HTTPResponse::HTTP_OK)
    {
        throw WebClientException("Error authenticating user\n");
    }

    auto password = execConn::Util::getJSONValue(content, "password");

    auto serial = deviceSerial;

    return Device(serial, password);

}

std::string WebClient::getDeviceTypeString(const DeviceType& deviceType)
{
    switch (deviceType)
    {
    case DeviceType::GENERIC:
        return "";

    case DeviceType::HEXIWEAR:
        return "HEXIWEAR";

    case DeviceType::WOLKSENSOR:
        return "WOLKSENSOR";

    default:
        return "";
    }
}
}
