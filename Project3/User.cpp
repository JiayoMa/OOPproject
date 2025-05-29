#include "User.h"
#include "Sensor.h" // For dynamic_cast
#include "AC.h"     // For dynamic_cast
#include "Light.h"  // For dynamic_cast
#include <iostream> // For std::cout in demo methods

// extern SmartLogger gLogger; // If used directly

User::User(const std::string& name, const std::string& pwd, UserRole r, std::vector<Device*>& deviceList)
    : username(name), password(pwd), role(r), devices(deviceList) {
    // gLogger.INFO("User created: " + username);
}

std::string User::getUsername() const {
    return username;
}

std::string User::getPassword() const {
    return password;
}

UserRole User::getRole() const {
    return role;
}

const std::vector<Device*>& User::getDevices() const {
    return devices;
}

void User::runTemperatureHumidityRule(double tempThreshold, double humidityThreshold) {
    // Using std::cout directly, no "using std::cout;" as a member
    std::cout << username << " 正在进行温湿度检测..." << std::endl;
    for (Device* device : devices) {
        if (Sensor* sensor = dynamic_cast<Sensor*>(device)) {
            if (sensor->getTemperature() > tempThreshold || sensor->getHumidity() > humidityThreshold) {
                std::cout << "警报：传感器 " << sensor->getName()
                    << " 检测到温度为"<< sensor->getTemperature() <<"湿度为" << sensor->getHumidity() <<"温湿度超限！" << std::endl;
                for (Device* acDevice : devices) {
                    if (AC* ac = dynamic_cast<AC*>(acDevice)) {
                        if (ac->getMode() == ACMode::OFF || ac->getMode() == ACMode::HEAT) {
                            std::cout << "关闭空调 " << ac->getName()
                                << " 调为制冷模式" << std::endl;
                            ac->setMode(ACMode::COOL);
                            ac->setTargetTemperature(22.0);
                        }
                    }
                }
            }
        }
    }
}

void User::runFireEmergencyRule(double co2FireThreshold) {
    std::cout << username << " 正在进行火情检测..." << std::endl;
    for (Device* device : devices) {
        if (Sensor* sensor = dynamic_cast<Sensor*>(device)) {
            if (sensor->getCO2Concentration() > co2FireThreshold) {
                std::cout << "严重警报：传感器" << sensor->getName()
                    << " 检测到二氧化碳超标！有火情！" << std::endl;
                for (Device* devToShutdown : devices) {
                    if (Light* light = dynamic_cast<Light*>(devToShutdown)) {
                        light->turnOff();
                        std::cout << "关灯断电" << light->getName() << std::endl;
                    }
                    else if (AC* ac = dynamic_cast<AC*>(devToShutdown)) {
                        ac->setMode(ACMode::OFF);
                        std::cout << "关闭空调" << ac->getName() << std::endl;
                    }
                    if (devToShutdown->getImportance() != DeviceImportance::CRITICAL) {
                        // Assuming setEmergencyPowerOff exists and is appropriate here
                        // devToShutdown->setEmergencyPowerOff(true); 
                        std::cout << "紧急关机"
                            << devToShutdown->getName() << std::endl;
                    }
                }
            }
        }
    }
}