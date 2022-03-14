#pragma once

#include "loadinterface.h"
#include "daikincontroller.h"

class DaikinLoad : public LoadInterface
{
public:
    DaikinLoad(const std::string &address);

    void enable() {
        daikinController.setParams(1, ControlMode::AUTO, 25, FanMode::AUTO, WingMode::BOTH, 40);
    };

    void disable() {
        daikinController.setParams(0);
    };

    double getPowerThreshold() const {
        return 0.7;
    };

    DaikinController daikinController;
};
