#include "../include/Light.h"
#include <iostream> 

Light::Light(int id, const std::string& name, DeviceImportance importance, double powerConsumption,
    bool on, int bright)
    : Device(id, name, importance, powerConsumption), isOn(on), brightness(bright) {
}

Light::~Light() {
    // std::cout << "灯具 " << name << " (ID: " << id << ") 已销毁。" << std::endl;
}

void Light::updateStatus() {
    std::cout << "正在更新灯具 " << name << " (ID: " << id << ") 的状态:" << std::endl;
    std::cout << "  当前状态: " << (isOn ? "开" : "关") << std::endl;
    if (isOn) {
        std::cout << "  亮度: " << brightness << "%" << std::endl;
    }
}

void Light::displayInfo() const {
    Device::displayInfo();
    std::cout << ", 状态: " << (isOn ? "开" : "关");
    if (isOn) {
        std::cout << ", 亮度: " << brightness << "%";
    }
}

void Light::turnOn() {
    isOn = true;
    std::cout << name << " (ID: " << id << ") 已开启。" << std::endl;
}

void Light::turnOff() {
    isOn = false;
    std::cout << name << " (ID: " << id << ") 已关闭。" << std::endl;
}

void Light::setBrightness(int level) {
    if (level >= 0 && level <= 100) {
        brightness = level;
        std::cout << name << " (ID: " << id << ") 亮度已设置为 " << brightness << "%。" << std::endl;
        if (brightness > 0 && !isOn) {
            turnOn();
        }
        else if (brightness == 0 && isOn) {
            turnOff();
        }
    }
    else {
        std::cout << "无效的亮度级别。必须在0到100之间。" << std::endl;
    }
}

bool Light::getIsOn() const { return isOn; }
int Light::getBrightness() const { return brightness; }

std::istream& operator>>(std::istream& is, Light& light) {
    is >> static_cast<Device&>(light);
    std::cout << "灯是否开启? (1 代表是, 0 代表否): ";
    int onState;
    is >> onState;
    light.isOn = (onState == 1);
    if (light.isOn) {
        std::cout << "请输入亮度 (0-100%): ";
        is >> light.brightness;
        if (light.brightness < 0) light.brightness = 0;
        if (light.brightness > 100) light.brightness = 100;
    }
    else {
        light.brightness = 0;
    }
    return is;
}