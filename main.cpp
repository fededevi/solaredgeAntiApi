#include "solaredgerequest.h"
#include "daikincontroller.h"
#include "loadinterface.h"
#include "daikinload.h"

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

    std::vector<LoadInterface *> loads{
                new DummyLoad(),
                new DaikinLoad("http://192.168.1.61/"),
                new DaikinLoad("http://192.168.1.231/"),
                new DummyLoad()
    };

    int level = 0;
    while (true) {
        std::string jsonString = ser.request();
        Json json = Json::parse(jsonString)["siteCurrentPowerFlow"];
        std::string storageStatus = json["STORAGE"].value("status", "unknown");
        double loadCurrentPower = json["LOAD"].value("currentPower", 0.0);
        double pvCurrentPower = json["PV"].value("currentPower", 0.0);
        double availablePower = pvCurrentPower - loadCurrentPower;
        std::cout << "Net power: " << availablePower << ". ";

        level = std::max(std::min(level, static_cast<int>(loads.size())-1),0);

        LoadInterface & li = *loads[level];
        if ( li.getPowerThreshold() < availablePower ) {
            level++;
            std::cout << "Enabling load " << level << std::endl;
            li.enable();
        }

        if ( availablePower < 0 ) {
            std::cout << "Disabling load " << level << std::endl;
            li.disable();
            level--;
        }

        std::this_thread::sleep_for(std::chrono::seconds(240));
    }

}
