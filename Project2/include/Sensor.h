#pragma once
#ifndef SENSOR_H
#define SENSOR_H

#include "Device.h"

class Sensor : public Device {
private:
    double temperature;     // Celsius
    double humidity;        // Percentage
    double co2Concentration; // ppm or percentage (as per your requirement, let's use percentage for fire detection)

public:
    Sensor(int id, const std::string& name, DeviceImportance importance, double powerConsumption,
           double temp = 20.0, double hum = 50.0, double co2 = 0.04); // 0.04% is typical atmospheric CO2

    ~Sensor() override;

    void updateStatus() override;
    void displayInfo() const override;

    // Getters
    double getTemperature() const;
    double getHumidity() const;
    double getCO2Concentration() const;

    // Setters
    void setTemperature(double temp);
    void setHumidity(double hum);
    void setCO2Concentration(double co2);

    friend std::istream& operator>>(std::istream& is, Sensor& sensor);
};

#endif // SENSOR_H