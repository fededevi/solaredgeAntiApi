#include "httprequest.h"

HttpRequest::HttpRequest(const std::string &_url, const std::map<std::string, std::string> &_cookies)
    : url(_url)
    , cookies(_cookies)
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

    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPAUTH, (long)CURLAUTH_ANY);

    for (const auto & cookie : cookies) {
        std::string cookieStr = cookie.first + "=" + cookie.second + ";";
        curl_easy_setopt(curl, CURLOPT_COOKIE, cookieStr.c_str());
    }

    std::string s = "";
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);

    CURLcode res = curl_easy_perform(curl);

    if(res != CURLE_OK) {
        s = "";
    }

    curl_easy_cleanup(curl);
    curl_global_cleanup();
    return s;
}
