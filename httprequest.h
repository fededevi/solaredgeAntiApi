#pragma once

#include <curl/curl.h>
#include <functional>
#include <string>
#include <map>

class HttpRequest
{

public:
    HttpRequest(const std::string &url);
    std::string request();

    HttpRequest & setOptGET();
    HttpRequest & setOptPOST();
    HttpRequest & setCookies(const std::map<std::string, std::string> &newCookies);
    HttpRequest & setUrl(const std::string &newUrl);
    HttpRequest & setParameters(const std::map<std::string, std::string> &newParameters);

private:
    std::string url;
    std::map<std::string, std::string> cookies;
    std::map<std::string, std::string> parameters;

    CURLoption option = CURLOPT_HTTPGET;

};
