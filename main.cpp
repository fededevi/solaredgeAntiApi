#include "solaredgerequest.h"
#include "daikincontroller.h"

#include <chrono>
#include <iostream>
#include <thread>
#include <map>

#include "json.hpp"

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

    int level = 0;
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

        double loadCurrentPower = json["LOAD"].value("currentPower", 0.0);
        std::cout << "LOAD.currentPower: " << loadCurrentPower << std::endl;

        double pvCurrentPower = json["PV"].value("currentPower", 0.0);
        std::cout << "PV.currentPower: " << pvCurrentPower << std::endl;

        double usedPower = level * 750;
        double availableEnergy = pvCurrentPower - loadCurrentPower + usedPower;
        std::cout << "availableEnergy: " << availableEnergy << std::endl;

        if ( storageStatus == "Discharging" || storageChargeLevel < 10) {
            std::cout << "Decrease Level" << std::endl;
            level--;
        } else if ( availableEnergy > 0.5 ) {
            std::cout << "Increase Level" << std::endl;
            level++;
        }

        level = std::max(std::min(level, 3), 0);
        std::cout << "Level " <<  level <<std::endl;

        switch (level) {
        case 0:
            std::cout << ctl1.setParams(0) << std::endl;
            std::cout << ctl2.setParams(0) << std::endl;
            break;
        case 1:
            std::cout << ctl1.setParams(1, ControlMode::AUTO, 28, FanMode::AUTO, WingMode::BOTH, 40) << std::endl;
            std::cout << ctl2.setParams(0) << std::endl;
            break;
        case 2:
            std::cout << ctl1.setParams(1, ControlMode::AUTO, 28, FanMode::AUTO, WingMode::BOTH, 40) << std::endl;
            std::cout << ctl2.setParams(1, ControlMode::AUTO, 28, FanMode::AUTO, WingMode::BOTH, 40) << std::endl;
            break;
        case 3:
            std::cout << ctl1.setParams(1, ControlMode::AUTO, 30, FanMode::LEVEL_5, WingMode::BOTH, 40) << std::endl;
            std::cout << ctl2.setParams(1, ControlMode::AUTO, 30, FanMode::LEVEL_5, WingMode::BOTH, 40) << std::endl;
            break;
        }

        std::this_thread::sleep_for(std::chrono::seconds(240));
    }

}
