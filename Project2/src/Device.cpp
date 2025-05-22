#include "../include/Device.h"
#include <iostream> // Make sure iostream is included if not alreadypowerConsumption
#include <sstream> //
Device::Device(int id, const std::string& name, DeviceImportance importance, double powerConsumption)
    : id(id), name(name), importance(importance), powerConsumption(powerConsumption), emergencyPowerOff(false) {
}
// 这就是 importanceToString 辅助函数
std::string importanceToString(DeviceImportance imp) { // 用于保存文件
    return std::to_string(static_cast<int>(imp)); // 将枚举值转换为其底层的整数，再转换为字符串
}
std::string Device::toFileString() const {
    std::stringstream ss;
    // 注意：类型字符串 (Sensor, Light, AC) 需要在子类中添加，或者在 saveDevicesToFile 中通过 dynamic_cast 判断
    // 这里只处理通用部分，不包含类型字符串和前导逗号
    ss << getId() << ","
        << getName() << ","
        << importanceToString(getImportance()) << "," // 需要 importanceToString 辅助函数
        << getPowerConsumption(); // 假设功耗可以直接输出
    return ss.str();
}
Device::~Device() {
    // std::cout << "设备 " << name << " (ID: " << id << ") 已销毁。" << std::endl;
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
    std::cout << "设备ID: " << id << ", 名称: " << name
        << ", 重要程度: ";
    switch (importance) {
    case DeviceImportance::LOW: std::cout << "低"; break;
    case DeviceImportance::MEDIUM: std::cout << "中"; break;
    case DeviceImportance::HIGH: std::cout << "高"; break;
    case DeviceImportance::CRITICAL: std::cout << "危急"; break;
    }
    std::cout << ", 能耗: " << powerConsumption << " 单位"
        << ", 紧急断电: " << (emergencyPowerOff ? "是" : "否");
}

std::ostream& operator<<(std::ostream& os, const Device& device) {
    device.displayInfo();
    return os;
}

std::istream& operator>>(std::istream& is, Device& device) {
    std::cout << "请输入设备ID: ";
    is >> device.id;
    std::cout << "请输入设备名称: ";
    is.ignore();
    std::getline(is, device.name);
    std::cout << "请输入能耗 (单位): ";
    is >> device.powerConsumption;
    // 重要程度通常通过菜单或特定逻辑设置，此处简化
    return is;
}
DeviceImportance stringToImportance(const std::string& s) {
    try {
        int val = std::stoi(s);
        if (val >= static_cast<int>(DeviceImportance::LOW) && val <= static_cast<int>(DeviceImportance::CRITICAL)) {
            return static_cast<DeviceImportance>(val);
        }
        std::cerr << "警告: 重要性值 '" << s << "' 超出预期范围。默认为 中。" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "警告: 重要性值 '" << s << "' 无效。默认为 中。错误: " << e.what() << std::endl;
    }
    return DeviceImportance::MEDIUM;
}

