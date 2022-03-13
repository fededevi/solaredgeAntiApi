#pragma once

#include "httprequest.h"

class SolarEdgeRequest : public HttpRequest
{

public:
    SolarEdgeRequest( const std::string & url, const std::string & token );

    std::string getJsonData();
};
