#pragma once
#ifndef DEVICEFACTORY_H
#define DEVICEFACTORY_H

#include "Device.h" 
#include "Sensor.h"
#include "Light.h"
#include "AC.h"
#include <string>
#include <vector>
#include <stdexcept> 

struct DeviceParams {
    int id;
    std::string name;
    DeviceImportance importance;
    double powerConsumption;

    // Sensor specific
    double temperature;
    double humidity;
    double co2Concentration;

    // Light specific
    bool isOn;
    int brightness;

    // AC specific
    ACMode acMode;
    double targetTemperature;
    FanSpeed fanSpeed;

    DeviceParams() : id(0), name("ƒ¨»œ…Ë±∏"), importance(DeviceImportance::MEDIUM), powerConsumption(0.0), // Changed "DefaultDevice"
        temperature(20.0), humidity(50.0), co2Concentration(0.0004),
        isOn(false), brightness(50),
        acMode(ACMode::OFF), targetTemperature(22.0), fanSpeed(FanSpeed::AUTO) {
    }
};


class DeviceFactory {
public:
    virtual ~DeviceFactory() = default;
    virtual Device* createDevice() = 0;
    virtual Device* createDeviceWithParams(const DeviceParams& params) = 0;
};

class SensorFactory : public DeviceFactory {
public:
    Device* createDevice() override;
    Device* createDeviceWithParams(const DeviceParams& params) override;
};

class LightFactory : public DeviceFactory {
public:
    Device* createDevice() override;
    Device* createDeviceWithParams(const DeviceParams& params) override;
};

class ACFactory : public DeviceFactory {
public:
    Device* createDevice() override;
    Device* createDeviceWithParams(const DeviceParams& params) override;
};

#endif // DEVICEFACTORY_H