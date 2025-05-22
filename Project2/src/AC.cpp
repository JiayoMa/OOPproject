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

#endif 
#include <iostream> 

AC::AC(int id, const std::string& name, DeviceImportance importance, double powerConsumption,
    ACMode mode, double temp, FanSpeed speed)
    : Device(id, name, importance, powerConsumption), mode(mode), targetTemperature(temp), fanSpeed(speed) {
}

AC::~AC() {
    
}

void AC::updateStatus() {
    std::cout << "正在更新空调 " << name << " (ID: " << id << ") 的状态:" << std::endl;
    std::cout << "  当前模式: ";
    switch (mode) {
    case ACMode::COOL: std::cout << "制冷"; break;
    case ACMode::HEAT: std::cout << "制热"; break;
    case ACMode::FAN: std::cout << "送风"; break;
    case ACMode::OFF: std::cout << "关闭"; break;
    }
    std::cout << std::endl;
    if (mode != ACMode::OFF) {
        std::cout << "  目标温度: " << targetTemperature << " C" << std::endl;
        std::cout << "  风速: ";
        switch (fanSpeed) {
        case FanSpeed::LOW: std::cout << "低"; break;
        case FanSpeed::MEDIUM: std::cout << "中"; break;
        case FanSpeed::HIGH: std::cout << "高"; break;
        case FanSpeed::AUTO: std::cout << "自动"; break;
        }
        std::cout << std::endl;
    }
}

void AC::displayInfo() const {
    Device::displayInfo();
    std::cout << ", 模式: ";
    switch (mode) {
    case ACMode::COOL: std::cout << "制冷"; break;
    case ACMode::HEAT: std::cout << "制热"; break;
    case ACMode::FAN: std::cout << "送风"; break;
    case ACMode::OFF: std::cout << "关闭"; break;
    }
    if (mode != ACMode::OFF) {
        std::cout << ", 目标温度: " << targetTemperature << " C"
            << ", 风速: ";
        switch (fanSpeed) {
        case FanSpeed::LOW: std::cout << "低"; break;
        case FanSpeed::MEDIUM: std::cout << "中"; break;
        case FanSpeed::HIGH: std::cout << "高"; break;
        case FanSpeed::AUTO: std::cout << "自动"; break;
        }
    }
}

void AC::setMode(ACMode m) {
    mode = m;
    std::cout << name << " (ID: " << id << ") 模式已设置为 ";
    switch (mode) {
    case ACMode::COOL: std::cout << "制冷"; break;
    case ACMode::HEAT: std::cout << "制热"; break;
    case ACMode::FAN: std::cout << "送风"; break;
    case ACMode::OFF: std::cout << "关闭"; break;
    }
    std::cout << "。" << std::endl;
}

void AC::setTargetTemperature(double temp) {
    targetTemperature = temp;
    std::cout << name << " (ID: " << id << ") 目标温度已设置为 " << targetTemperature << " C。" << std::endl;
}

void AC::setFanSpeed(FanSpeed speed) {
    fanSpeed = speed;
    std::cout << name << " (ID: " << id << ") 风速已设置为 ";
    switch (fanSpeed) {
    case FanSpeed::LOW: std::cout << "低"; break;
    case FanSpeed::MEDIUM: std::cout << "中"; break;
    case FanSpeed::HIGH: std::cout << "高"; break;
    case FanSpeed::AUTO: std::cout << "自动"; break;
    }
    std::cout << "。" << std::endl;
}

ACMode AC::getMode() const { return mode; }
double AC::getTargetTemperature() const { return targetTemperature; }
FanSpeed AC::getFanSpeed() const { return fanSpeed; }

std::istream& operator>>(std::istream& is, AC& ac) {
    is >> static_cast<Device&>(ac);
    int modeChoice, fanChoice;
    std::cout << "请输入空调模式 (0:关闭, 1:制冷, 2:制热, 3:送风): ";
    is >> modeChoice;
    ac.mode = static_cast<ACMode>(modeChoice);

    if (ac.mode != ACMode::OFF) {
        std::cout << "请输入目标温度 (C): ";
        is >> ac.targetTemperature;
        std::cout << "请输入风速 (0:低, 1:中, 2:高, 3:自动): ";
        is >> fanChoice;
        ac.fanSpeed = static_cast<FanSpeed>(fanChoice);
    }
    return is;
}