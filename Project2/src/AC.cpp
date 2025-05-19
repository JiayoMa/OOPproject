#include "../include/AC.h"
#include <iostream> 

AC::AC(int id, const std::string& name, DeviceImportance importance, double powerConsumption,
    ACMode mode, double temp, FanSpeed speed)
    : Device(id, name, importance, powerConsumption), mode(mode), targetTemperature(temp), fanSpeed(speed) {
}

AC::~AC() {
    // std::cout << "�յ� " << name << " (ID: " << id << ") �����١�" << std::endl;
}

void AC::updateStatus() {
    std::cout << "���ڸ��¿յ� " << name << " (ID: " << id << ") ��״̬:" << std::endl;
    std::cout << "  ��ǰģʽ: ";
    switch (mode) {
    case ACMode::COOL: std::cout << "����"; break;
    case ACMode::HEAT: std::cout << "����"; break;
    case ACMode::FAN: std::cout << "�ͷ�"; break;
    case ACMode::OFF: std::cout << "�ر�"; break;
    }
    std::cout << std::endl;
    if (mode != ACMode::OFF) {
        std::cout << "  Ŀ���¶�: " << targetTemperature << " C" << std::endl;
        std::cout << "  ����: ";
        switch (fanSpeed) {
        case FanSpeed::LOW: std::cout << "��"; break;
        case FanSpeed::MEDIUM: std::cout << "��"; break;
        case FanSpeed::HIGH: std::cout << "��"; break;
        case FanSpeed::AUTO: std::cout << "�Զ�"; break;
        }
        std::cout << std::endl;
    }
}

void AC::displayInfo() const {
    Device::displayInfo();
    std::cout << ", ģʽ: ";
    switch (mode) {
    case ACMode::COOL: std::cout << "����"; break;
    case ACMode::HEAT: std::cout << "����"; break;
    case ACMode::FAN: std::cout << "�ͷ�"; break;
    case ACMode::OFF: std::cout << "�ر�"; break;
    }
    if (mode != ACMode::OFF) {
        std::cout << ", Ŀ���¶�: " << targetTemperature << " C"
            << ", ����: ";
        switch (fanSpeed) {
        case FanSpeed::LOW: std::cout << "��"; break;
        case FanSpeed::MEDIUM: std::cout << "��"; break;
        case FanSpeed::HIGH: std::cout << "��"; break;
        case FanSpeed::AUTO: std::cout << "�Զ�"; break;
        }
    }
}

void AC::setMode(ACMode m) {
    mode = m;
    std::cout << name << " (ID: " << id << ") ģʽ������Ϊ ";
    switch (mode) {
    case ACMode::COOL: std::cout << "����"; break;
    case ACMode::HEAT: std::cout << "����"; break;
    case ACMode::FAN: std::cout << "�ͷ�"; break;
    case ACMode::OFF: std::cout << "�ر�"; break;
    }
    std::cout << "��" << std::endl;
}

void AC::setTargetTemperature(double temp) {
    targetTemperature = temp;
    std::cout << name << " (ID: " << id << ") Ŀ���¶�������Ϊ " << targetTemperature << " C��" << std::endl;
}

void AC::setFanSpeed(FanSpeed speed) {
    fanSpeed = speed;
    std::cout << name << " (ID: " << id << ") ����������Ϊ ";
    switch (fanSpeed) {
    case FanSpeed::LOW: std::cout << "��"; break;
    case FanSpeed::MEDIUM: std::cout << "��"; break;
    case FanSpeed::HIGH: std::cout << "��"; break;
    case FanSpeed::AUTO: std::cout << "�Զ�"; break;
    }
    std::cout << "��" << std::endl;
}

ACMode AC::getMode() const { return mode; }
double AC::getTargetTemperature() const { return targetTemperature; }
FanSpeed AC::getFanSpeed() const { return fanSpeed; }

std::istream& operator>>(std::istream& is, AC& ac) {
    is >> static_cast<Device&>(ac);
    int modeChoice, fanChoice;
    std::cout << "������յ�ģʽ (0:�ر�, 1:����, 2:����, 3:�ͷ�): ";
    is >> modeChoice;
    ac.mode = static_cast<ACMode>(modeChoice);

    if (ac.mode != ACMode::OFF) {
        std::cout << "������Ŀ���¶� (C): ";
        is >> ac.targetTemperature;
        std::cout << "��������� (0:��, 1:��, 2:��, 3:�Զ�): ";
        is >> fanChoice;
        ac.fanSpeed = static_cast<FanSpeed>(fanChoice);
    }
    return is;
}