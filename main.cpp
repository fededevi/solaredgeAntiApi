#include "solaredgerequest.h"

#include <chrono>
#include <iostream>
#include <thread>

int main(int argc, char **argv)
{
    if (argc < 3) {
        std::cout << "please provide parameters: monitoring url and SPRING_SECURITY_REMEMBER_ME_COOKIE\n" \
                     "They look like this:\n" \
                     "URL: https://monitoring.solaredge.com/solaredge-apigw/api/site/215645/currentPowerFlow.json\n" \
                     "TOKEN: ZmFiaW9kZXZpZ2lsaUB5YWhvby5pdDozNzk0NjcwMsteOTAyOjBiNGY0MWQ1NGJlYzUwYmIxZAFjZGQ1NzcwZWM3OWZj" << std::endl;
    }

    std::string url(argv[1]);
    std::string token(argv[2]);

    SolarEdgeRequest ser(url, token);

    while (true) {
        std::cout << "Req" << ser.request() << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    }

}
