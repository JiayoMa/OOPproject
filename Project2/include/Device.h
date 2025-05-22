#pragma once
#ifndef DEVICE_H
#define DEVICE_H

#include <string>
#include <iostream>
#include <vector> // For params in factory

// Forward declaration for DeviceFactory to avoid circular dependency
class DeviceFactory;

// Enum for device types, useful for factories and containers
enum class DeviceType {
    SENSOR,
    LIGHT,
    AC
};

// Enum for device importance (example)
enum class DeviceImportance {
    LOW,
    MEDIUM,
    HIGH,
    CRITICAL
};
std::string importanceToString(DeviceImportance imp);
class Device {
protected:
    int id;
    std::string name;
    DeviceImportance importance;
    double powerConsumption; // Assuming kWh or similar unit
    bool emergencyPowerOff; // For emergency scenarios

public:
    // Constructor
    Device(int id, const std::string& name, DeviceImportance importance, double powerConsumption);

    // Virtual destructor
    virtual ~Device();
    virtual std::string toFileString() const; // 基类实现通用部分
    // Pure virtual function for status update (to be implemented by derived classes)
    virtual void updateStatus() = 0;

    // Getters
    int getId() const;
    std::string getName() const;
    DeviceImportance getImportance() const;
    double getPowerConsumption() const;
    bool isEmergencyPowerOff() const;

    // Setters
    void setId(int id);
    void setName(const std::string& name);
    void setImportance(DeviceImportance importance);
    void setPowerConsumption(double consumption);
    void setEmergencyPowerOff(bool status);


    // Virtual function to display device info, can be overridden
    virtual void displayInfo() const;

    // Friend function for overloading output operator
    friend std::ostream& operator<<(std::ostream& os, const Device& device);
    // Friend function for overloading input operator (basic example)
    friend std::istream& operator>>(std::istream& is, Device& device);
};
DeviceImportance stringToImportance(const std::string& s);
#endif // DEVICE_H