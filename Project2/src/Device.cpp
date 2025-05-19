#include "../include/Device.h"
#include <iostream> // Make sure iostream is included if not already

Device::Device(int id, const std::string& name, DeviceImportance importance, double powerConsumption)
    : id(id), name(name), importance(importance), powerConsumption(powerConsumption), emergencyPowerOff(false) {
}

Device::~Device() {
    // std::cout << "�豸 " << name << " (ID: " << id << ") �����١�" << std::endl;
}

int Device::getId() const { return id; }
std::string Device::getName() const { return name; }
DeviceImportance Device::getImportance() const { return importance; }
double Device::getPowerConsumption() const { return powerConsumption; }
bool Device::isEmergencyPowerOff() const { return emergencyPowerOff; }

void Device::setId(int id) { this->id = id; }
void Device::setName(const std::string& name) { this->name = name; }
void Device::setImportance(DeviceImportance importance) { this->importance = importance; }
void Device::setPowerConsumption(double consumption) { this->powerConsumption = consumption; }
void Device::setEmergencyPowerOff(bool status) { this->emergencyPowerOff = status; }


void Device::displayInfo() const {
    std::cout << "�豸ID: " << id << ", ����: " << name
        << ", ��Ҫ�̶�: ";
    switch (importance) {
    case DeviceImportance::LOW: std::cout << "��"; break;
    case DeviceImportance::MEDIUM: std::cout << "��"; break;
    case DeviceImportance::HIGH: std::cout << "��"; break;
    case DeviceImportance::CRITICAL: std::cout << "Σ��"; break;
    }
    std::cout << ", �ܺ�: " << powerConsumption << " ��λ"
        << ", �����ϵ�: " << (emergencyPowerOff ? "��" : "��");
}

std::ostream& operator<<(std::ostream& os, const Device& device) {
    device.displayInfo();
    return os;
}

std::istream& operator>>(std::istream& is, Device& device) {
    std::cout << "�������豸ID: ";
    is >> device.id;
    std::cout << "�������豸����: ";
    is.ignore();
    std::getline(is, device.name);
    std::cout << "�������ܺ� (��λ): ";
    is >> device.powerConsumption;
    // ��Ҫ�̶�ͨ��ͨ���˵����ض��߼����ã��˴���
    return is;
}