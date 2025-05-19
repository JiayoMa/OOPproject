#pragma once
#ifndef USER_H
#define USER_H

#include <string>
#include <vector>
#include "Device.h" // For Device pointer vector
#include "Sensor.h" // For dynamic_cast
#include "AC.h"     // For dynamic_cast and AC control
#include "Light.h"  // For dynamic_cast
#include "SmartLogger.h" // For logging

// Forward declare gLogger if it's global and defined in main.cpp
extern SmartLogger gLogger;


class User {
private:
    std::string username;
    std::vector<Device*>& devices; // Reference to the main list of devices

public:
    User(const std::string& name, std::vector<Device*>& deviceList);

    std::string getUsername() const;

    // Scene simulation methods
    void runTemperatureHumidityRule(double tempThreshold = 30.0, double humidityThreshold = 70.0);
    void runFireEmergencyRule(double co2FireThreshold = 0.06); // 6% CO2 concentration

    // Later: Add methods for permission management if needed
};

#endif // USER_H