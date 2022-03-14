#include "solaredgerequest.h"
#include "daikincontroller.h"

#include <chrono>
#include <iostream>
#include <thread>
#include <map>

#include "json/single_include/nlohmann/json.hpp"

using Json = nlohmann::json;

int main(int argc, char **argv)
{
    if (argc < 3) {
        std::cout << "please provide parameters: monitoring url and SPRING_SECURITY_REMEMBER_ME_COOKIE\n" \
                     "They look like this:\n" \
                     "URL: https://monitoring.solaredge.com/solaredge-apigw/api/site/215645/currentPowerFlow.json\n" \
                     "TOKEN: ZmFiaW9kZXZpZ2lsaUB5YWhvby5pdDozNzk0NjcwMsteOTAyOjBiNGY0MWQ1NGJlYzUwYmIxZAFjZGQ1NzcwZWM3OWZj" << std::endl;
        return 1;
    }

    std::string url(argv[1]);
    std::string token(argv[2]);

    SolarEdgeRequest ser(url, token);
    DaikinController ctl1("http://192.168.1.61/");
    DaikinController ctl2("http://192.168.1.62/");

    while (true) {

        std::cout << std::endl << "Request status... " << std::endl;
        std::string jsonString = ser.request();
        std::cout << ser.request() << std::endl;

        Json json = Json::parse(jsonString)["siteCurrentPowerFlow"];

        int updateRefreshRate = json.value("updateRefreshRate", 3);
        std::cout << "Using refresh rate of " << updateRefreshRate << " seconds"<< std::endl;

        std::string storageStatus = json["STORAGE"].value("status", "unknown");
        std::cout << "STORAGE.status: " << storageStatus << std::endl;

        double storageCurrentPower = json["STORAGE"].value("currentPower", 0.0);
        std::cout << "STORAGE.currentPower: " << storageCurrentPower << std::endl;

        int storageChargeLevel = json["STORAGE"].value("chargeLevel", 0);
        std::cout << "STORAGE.chargeLevel: " << storageChargeLevel << std::endl;

        double loadCurrentPower = json["LOAD"].value("currentPower", 0);
        std::cout << "LOAD.currentPower: " << loadCurrentPower << std::endl;

        double pvCurrentPower = json["PV"].value("currentPower", 0);
        std::cout << "PV.currentPower: " << pvCurrentPower << std::endl;

        double usedPower = 0;
        double availableEnergy = pvCurrentPower - loadCurrentPower + usedPower;



        if ( storageStatus == "Discharging" || storageChargeLevel < 10) {
            std::cout << "Turn off" << std::endl;
            std::cout << ctl1.setParams(0) << std::endl;
            std::cout << ctl2.setParams(0) << std::endl;
            usedPower = 0;
        } else if ( availableEnergy > 0.5 ) {
            std::cout << "Turn on" << std::endl;
            std::cout << ctl1.setParams(1, ControlMode::AUTO, 28, FanMode::AUTO, WingMode::BOTH, 40) << std::endl;
            std::cout << ctl2.setParams(1, ControlMode::AUTO, 28, FanMode::AUTO, WingMode::BOTH, 40) << std::endl;
            usedPower = 1.5;
        }

        std::this_thread::sleep_for(std::chrono::seconds(240));
    }

}
