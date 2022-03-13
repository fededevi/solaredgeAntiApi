#include "solaredgerequest.h"

#include <chrono>
#include <iostream>
#include <thread>

#include "json/single_include/nlohmann/json.hpp"

using Json = nlohmann::json;

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

    for (int i = 0; i < 3; i++) {
        std::string jsonString = ser.request();
        //std::cout << ser.request() << std::endl;

        Json json = Json::parse(jsonString)["siteCurrentPowerFlow"];

        int updateRefreshRate = json.value("updateRefreshRate", 3);

        std::string storageStatus = json["STORAGE"].value("status", "unknown");
        std::cout << "STORAGE.status: " << storageStatus << std::endl;

        double storageCurrentPower = json["STORAGE"].value("currentPower", 0.0);
        std::cout << "STORAGE.currentPower: " << storageCurrentPower << std::endl;

        int storageChargeLevel = json["STORAGE"].value("chargeLevel", 0);
        std::cout << "STORAGE.chargeLevel: " << storageChargeLevel << std::endl;

        std::this_thread::sleep_for(std::chrono::seconds(updateRefreshRate));
    }

}
