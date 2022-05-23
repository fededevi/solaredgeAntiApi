#pragma once

#include "loadinterface.h"
#include "daikincontroller.h"
#include <iostream>

class DaikinLoad : public LoadInterface
{
public:
    DaikinLoad(const std::string &address);

    void enable() {
        std::cout << "Turning on Daikin - " << daikinController.host << std::endl;
        daikinController.setParams(1, ControlMode::AUTO, 22, FanMode::LEVEL_5, WingMode::VERTICAL, 40);
    };

    void disable() {
        std::cout << "Turning off Daikin - " << daikinController.host << std::endl;
        daikinController.setParams(0);
    };

    double getPowerThreshold() const {
        return 0.7;
    };

    DaikinController daikinController;
};
