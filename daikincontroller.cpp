#include "daikincontroller.h"

DaikinController::DaikinController(const std::string &host)
    : host(host)
{

}

std::string DaikinController::setParams(bool pow, ControlMode mode, uint stemp, FanMode f_rate, WingMode f_dir, uint shum)
{
    HttpRequest request(host+"aircon/set_control_info");
    std::string fanmodeString; fanmodeString=static_cast<int>(f_rate);
    request.setOptGET().setParameters({
                                           {"pow", std::to_string(pow)},
                                           {"mode", std::to_string(static_cast<int>(mode))},
                                           {"stemp", std::to_string(stemp)},
                                           {"f_rate", fanmodeString},
                                           {"f_dir", std::to_string(static_cast<int>(f_dir))},
                                           {"shum", std::to_string(shum)}
                                       });
    return request.request();
}
