#pragma once

#include <curl/curl.h>
#include <functional>
#include <string>
#include <map>

class HttpRequest
{

public:
    HttpRequest(const std::string &url, const std::map<std::string, std::string > &cookies);
    std::string request();

private:
    std::string url;
    std::map<std::string, std::string > cookies;

};
