#include "../include/Sensor.h"
#include <iostream> 

Sensor::Sensor(int id, const std::string& name, DeviceImportance importance, double powerConsumption,
    double temp, double hum, double co2)
    : Device(id, name, importance, powerConsumption), temperature(temp), humidity(hum), co2Concentration(co2) {
}

Sensor::~Sensor() {
    // std::cout << "传感器 " << name << " (ID: " << id << ") 已销毁。" << std::endl;
}

void Sensor::updateStatus() {
    std::cout << "正在更新传感器 " << name << " (ID: " << id << ") 的状态:" << std::endl;
    temperature += (rand() % 3 - 1) * 0.1;
    humidity += (rand() % 5 - 2) * 0.5;
    co2Concentration += (rand() % 3 - 1) * 0.001;

    if (humidity < 0) humidity = 0;
    if (humidity > 100) humidity = 100;
    if (co2Concentration < 0) co2Concentration = 0;

    std::cout << "  新温度: " << temperature << " C" << std::endl;
    std::cout << "  新湿度: " << humidity << " %" << std::endl;
    std::cout << "  新CO2浓度: " << co2Concentration * 100 << " %" << std::endl;
}

void Sensor::displayInfo() const {
    Device::displayInfo();
    std::cout << ", 温度: " << temperature << " C"
        << ", 湿度: " << humidity << " %"
        << ", CO2浓度: " << co2Concentration * 100 << " %";
}

double Sensor::getTemperature() const { return temperature; }
double Sensor::getHumidity() const { return humidity; }
double Sensor::getCO2Concentration() const { return co2Concentration; }

void Sensor::setTemperature(double temp) { this->temperature = temp; }
void Sensor::setHumidity(double hum) { this->humidity = hum; }
void Sensor::setCO2Concentration(double co2) { this->co2Concentration = co2; }

std::istream& operator>>(std::istream& is, Sensor& sensor) {
    is >> static_cast<Device&>(sensor);
    std::cout << "请输入温度 (C): ";
    is >> sensor.temperature;
    std::cout << "请输入湿度 (%): ";
    is >> sensor.humidity;
    std::cout << "请输入CO2浓度 (例如, 0.06 代表 6%): ";
    is >> sensor.co2Concentration;
    return is;
}