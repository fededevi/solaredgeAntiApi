#pragma once

#include "httprequest.h"

enum class ControlMode {
    AUTO = 1,
    DEHUMDIFICATOR = 2,
    COLD = 3,
    HOT = 4,
    FAN = 6
};

enum class FanMode {
    AUTO = 'A',
    SILENCE = 'B',
    LEVEL_1 = '3',
    LEVEL_2 = '4',
    LEVEL_3 = '5',
    LEVEL_4 = '6',
    LEVEL_5 = '7'
};

enum class WingMode {
    NONE = 0,
    VERTICAL = 1,
    HORIZONTAL = 2,
    BOTH = 3
};

class DaikinController
{
public:
    DaikinController( const std::string & host );

    std::string setParams(bool pow = 1,
                   ControlMode mode = ControlMode::AUTO,
                   uint stemp = 25,
                   FanMode frate=FanMode::AUTO,
                   WingMode f_dir=WingMode::BOTH,
                   uint shum = 40);

    std::string host;
};

/*
 From https://github.com/ael-code/daikin-control
API System
Daikin original API use REST.

You can use GET http request to retrive informations and POST http request to apply new settings.

Uri                         GET	POST        desc
/common/basic_info          X               Provides Daikin account information (security issue!), software version, mac address and generic info
/common/get_remote_method	X               Provides information about polling system
/common/set_remote_method		X           Set information on the polling system (reduce remote time update ??)
/aircon/get_model_info      X               Provides model informarion
/aircon/get_control_info	X               Main Uri to request all current status parameters
/aircon/set_control_info	X	X           Main Uri to set status parameters (control almost all)
/aircon/get_sensor_info     X               Provides information on sensors (temperatures, humidity, power consumption)
/aircon/get_timer           X               ?
/aircon/set_timer           X	X           ?
/aircon/get_price           X               ?
/aircon/set_price           X   X           ?
/aircon/get_target          X               ?
/aircon/set_target          X   X           ?
/aircon/get_week_power      X               Provides weekly and today runtime information (in mn)
/aircon/get_year_power      X               Provides yearly runtime information
/aircon/get_program         X               ?
/aircon/set_program         X   X           ?
/aircon/get_scdltimer       X               Provides information about on/off weekly timer
/aircon/set_scdltimer		X   X           Set information about on/off weekly timer
/common/get_notify          X               ?
/common/set_notify          X   X           ?
/common/set_regioncode		X   X           ?
/common/set_led             X   X           ?
/common/reboot              X               reboot the AP

*/
