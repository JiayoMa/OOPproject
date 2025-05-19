#include "../include/Sensor.h"
#include <iostream> 

Sensor::Sensor(int id, const std::string& name, DeviceImportance importance, double powerConsumption,
    double temp, double hum, double co2)
    : Device(id, name, importance, powerConsumption), temperature(temp), humidity(hum), co2Concentration(co2) {
}

Sensor::~Sensor() {
    // std::cout << "������ " << name << " (ID: " << id << ") �����١�" << std::endl;
}

void Sensor::updateStatus() {
    std::cout << "���ڸ��´����� " << name << " (ID: " << id << ") ��״̬:" << std::endl;
    temperature += (rand() % 3 - 1) * 0.1;
    humidity += (rand() % 5 - 2) * 0.5;
    co2Concentration += (rand() % 3 - 1) * 0.001;

    if (humidity < 0) humidity = 0;
    if (humidity > 100) humidity = 100;
    if (co2Concentration < 0) co2Concentration = 0;

    std::cout << "  ���¶�: " << temperature << " C" << std::endl;
    std::cout << "  ��ʪ��: " << humidity << " %" << std::endl;
    std::cout << "  ��CO2Ũ��: " << co2Concentration * 100 << " %" << std::endl;
}

void Sensor::displayInfo() const {
    Device::displayInfo();
    std::cout << ", �¶�: " << temperature << " C"
        << ", ʪ��: " << humidity << " %"
        << ", CO2Ũ��: " << co2Concentration * 100 << " %";
}

double Sensor::getTemperature() const { return temperature; }
double Sensor::getHumidity() const { return humidity; }
double Sensor::getCO2Concentration() const { return co2Concentration; }

void Sensor::setTemperature(double temp) { this->temperature = temp; }
void Sensor::setHumidity(double hum) { this->humidity = hum; }
void Sensor::setCO2Concentration(double co2) { this->co2Concentration = co2; }

std::istream& operator>>(std::istream& is, Sensor& sensor) {
    is >> static_cast<Device&>(sensor);
    std::cout << "�������¶� (C): ";
    is >> sensor.temperature;
    std::cout << "������ʪ�� (%): ";
    is >> sensor.humidity;
    std::cout << "������CO2Ũ�� (����, 0.06 ���� 6%): ";
    is >> sensor.co2Concentration;
    return is;
}