#include "solaredgerequest.h"

SolarEdgeRequest::SolarEdgeRequest(const std::string &url, const std::string &token)
    :HttpRequest(url)
{
    setCookies({{"SPRING_SECURITY_REMEMBER_ME_COOKIE", token}});
    setOptGET();
}

std::string SolarEdgeRequest::getJsonData()
{
    return request();
}
