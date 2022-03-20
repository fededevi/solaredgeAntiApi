#include "solaredgerequest.h"
#include "averager.h"
#include "daikincontroller.h"
#include "daikinload.h"
#include "loadinterface.h"

#include <chrono>
#include <iostream>
#include <thread>

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

    std::string url(argv[1]);       //Solaredge monitoring api address
    std::string token(argv[2]);     //Solaredge login token (not the api key but the login token)


    std::vector<LoadInterface *> loads{
                new DummyLoad(),
                new DaikinLoad("http://192.168.1.61/"), //Daikin split local ip
                new DaikinLoad("http://192.168.1.231/"), //Daikin split local ip
                new DummyLoad()
    };

    for (auto & load : loads) load->disable(); //Disable all loads

    Averager netPowerAverage;
    SolarEdgeRequest ser(url, token);
    bool exit= false;

    std::thread solarEdgeThread([&](){
        std::cout << "   TIME   " << "\t\t" <<
                     "LOAD" << "\t" <<
                     "PV" << "\t" <<
                     "STRG" << "\t" <<
                     "AVLB" << "\t" <<
                     "GRID";
        while(!exit) {
            std::string jsonString = ser.request();
            try {
                Json json = Json::parse(jsonString)["siteCurrentPowerFlow"];
                //std::cout << jsonString << " " << std::flush;
                double loadCurrentPower = json["LOAD"].value("currentPower", 0.0);
                double pvCurrentPower = json["PV"].value("currentPower", 0.0);
                double storageCurrentPower = json["STORAGE"].value("currentPower", 0.0);
                double gridCurrentPower = json["GRID"].value("currentPower", 0.0);
                double availablePower = pvCurrentPower - loadCurrentPower;
                netPowerAverage.add(availablePower);
                std::cout << std::endl <<
                             time(nullptr) << "\t\t" <<
                             loadCurrentPower << "\t" <<
                             pvCurrentPower << "\t" <<
                             storageCurrentPower << "\t" <<
                             availablePower << "\t" <<
                             gridCurrentPower;
            }  catch ( ... ) { }
            std::this_thread::sleep_for(std::chrono::seconds(30));
        }
    });


    int level = 0;
    while (!exit) {
        std::this_thread::sleep_for(std::chrono::seconds(240));
        double averageNetPower = netPowerAverage.get();

        LoadInterface & li = *loads[level];
        if ( li.getPowerThreshold() < averageNetPower ) {
            level++;
            li.enable();
        }

        if ( averageNetPower < 0 ) {
            li.disable();
            level--;
        }

        level = std::max(std::min(level, static_cast<int>(loads.size())-1),0); // clamp level


    }

}
