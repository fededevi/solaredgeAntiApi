#include "httprequest.h"

HttpRequest::HttpRequest(const std::string &_url)
    : url(_url)
{

}

size_t writefunc(void *ptr, size_t size, size_t nmemb, std::string *s)
{
    s->append(static_cast<char *>(ptr), size*nmemb);
    return size*nmemb;
}

std::string HttpRequest::request()
{
    CURL *curl = nullptr;

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();

    std::string urlParams;
    for (const auto& param : parameters) {
        urlParams += param.first;
        urlParams += "=";
        urlParams += param.second;
        urlParams += "&";
    }
    urlParams = urlParams.substr(0, urlParams.size()-1);
    std::string finalurl = url + "?" + urlParams;

    curl_easy_setopt(curl, option, 1L);
    curl_easy_setopt(curl, CURLOPT_URL, (url+urlParams).c_str());
    //curl_easy_setopt(curl, CURLOPT_HTTPAUTH, (long)CURLAUTH_ANY);
    //curl_easy_setopt(curl, CURLOPT_, (long)CURLAUTH_ANY);

    for (const auto & cookie : cookies) {
        std::string cookieStr = cookie.first + "=" + cookie.second + ";";
        curl_easy_setopt(curl, CURLOPT_COOKIE, cookieStr.c_str());
    }

    std::string s = "";
    if (option == CURLOPT_HTTPGET) {
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
    }

    CURLcode res = curl_easy_perform(curl);

    if(res != CURLE_OK) {
        s = "";
    }

    curl_easy_cleanup(curl);
    curl_global_cleanup();
    return s;
}

HttpRequest &HttpRequest::setOptGET()
{
    option = CURLOPT_HTTPGET;
    return *this;
}

HttpRequest &HttpRequest::setOptPOST()
{
    option = CURLOPT_POSTFIELDS;
    return *this;
}

HttpRequest &HttpRequest::setCookies(const std::map<std::string, std::string> &newCookies)
{
    cookies = newCookies;
    return *this;
}

HttpRequest &HttpRequest::setUrl(const std::string &newUrl)
{
    url = newUrl;
    return *this;
}

HttpRequest & HttpRequest::setParameters(const std::map<std::string, std::string> &newParameters)
{
    parameters = newParameters;
    return *this;
}
