#include "../include/Light.h"
#include <iostream> 

Light::Light(int id, const std::string& name, DeviceImportance importance, double powerConsumption,
    bool on, int bright)
    : Device(id, name, importance, powerConsumption), isOn(on), brightness(bright) {
}

Light::~Light() {
    // std::cout << "�ƾ� " << name << " (ID: " << id << ") �����١�" << std::endl;
}

void Light::updateStatus() {
    std::cout << "���ڸ��µƾ� " << name << " (ID: " << id << ") ��״̬:" << std::endl;
    std::cout << "  ��ǰ״̬: " << (isOn ? "��" : "��") << std::endl;
    if (isOn) {
        std::cout << "  ����: " << brightness << "%" << std::endl;
    }
}

void Light::displayInfo() const {
    Device::displayInfo();
    std::cout << ", ״̬: " << (isOn ? "��" : "��");
    if (isOn) {
        std::cout << ", ����: " << brightness << "%";
    }
}

void Light::turnOn() {
    isOn = true;
    std::cout << name << " (ID: " << id << ") �ѿ�����" << std::endl;
}

void Light::turnOff() {
    isOn = false;
    std::cout << name << " (ID: " << id << ") �ѹرա�" << std::endl;
}

void Light::setBrightness(int level) {
    if (level >= 0 && level <= 100) {
        brightness = level;
        std::cout << name << " (ID: " << id << ") ����������Ϊ " << brightness << "%��" << std::endl;
        if (brightness > 0 && !isOn) {
            turnOn();
        }
        else if (brightness == 0 && isOn) {
            turnOff();
        }
    }
    else {
        std::cout << "��Ч�����ȼ��𡣱�����0��100֮�䡣" << std::endl;
    }
}

bool Light::getIsOn() const { return isOn; }
int Light::getBrightness() const { return brightness; }

std::istream& operator>>(std::istream& is, Light& light) {
    is >> static_cast<Device&>(light);
    std::cout << "���Ƿ���? (1 ������, 0 �����): ";
    int onState;
    is >> onState;
    light.isOn = (onState == 1);
    if (light.isOn) {
        std::cout << "���������� (0-100%): ";
        is >> light.brightness;
        if (light.brightness < 0) light.brightness = 0;
        if (light.brightness > 100) light.brightness = 100;
    }
    else {
        light.brightness = 0;
    }
    return is;
}