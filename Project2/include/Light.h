#pragma once
#ifndef LIGHT_H
#define LIGHT_H

#include "Device.h"

class Light : public Device {
private:
    bool isOn;
    int brightness; // Percentage 0-100

public:
    Light(int id, const std::string& name, DeviceImportance importance, double powerConsumption,
          bool on = false, int bright = 50);

    ~Light() override;

    void updateStatus() override;
    void displayInfo() const override;

    // Light specific methods
    void turnOn();
    void turnOff();
    void setBrightness(int level);

    // Getters
    bool getIsOn() const;
    int getBrightness() const;

    friend std::istream& operator>>(std::istream& is, Light& light);
};

#endif // LIGHT_H