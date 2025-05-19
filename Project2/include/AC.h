#pragma once
#ifndef AC_H
#define AC_H

#include "Device.h"

enum class ACMode {
    COOL,
    HEAT,
    FAN,
    OFF
};

enum class FanSpeed {
    LOW,
    MEDIUM,
    HIGH,
    AUTO
};

class AC : public Device {
private:
    ACMode mode;
    double targetTemperature; // Celsius
    FanSpeed fanSpeed;

public:
    AC(int id, const std::string& name, DeviceImportance importance, double powerConsumption,
       ACMode mode = ACMode::OFF, double temp = 22.0, FanSpeed speed = FanSpeed::AUTO);

    ~AC() override;

    void updateStatus() override;
    void displayInfo() const override;

    // AC specific methods
    void setMode(ACMode m);
    void setTargetTemperature(double temp);
    void setFanSpeed(FanSpeed speed);

    // Getters
    ACMode getMode() const;
    double getTargetTemperature() const;
    FanSpeed getFanSpeed() const;

    friend std::istream& operator>>(std::istream& is, AC& ac);
};

#endif // AC_H