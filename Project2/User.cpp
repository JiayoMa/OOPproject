#include "User.h"
#include <iostream> // For direct cout if logger not fully integrated everywhere

User::User(const std::string& name, std::vector<Device*>& deviceList)
    : username(name), devices(deviceList) {
    gLogger.INFO("�û� '" + username + "' �ѳ�ʼ����");
}

std::string User::getUsername() const {
    return username;
}

void User::runTemperatureHumidityRule(double tempThreshold, double humidityThreshold) {
    std::cout << "����ģ��: �����ʪ�ȹ���" << std::endl;
    std::cout << "��־�ļ������������������־�ļ�" << std::endl;
    gLogger.INFO("����ģ��: �����ʪ�ȹ���", -1);
    bool actionTaken = false;

    for (Device* device : devices) {
        Sensor* sensor = dynamic_cast<Sensor*>(device);
        if (sensor) {
            gLogger.DEBUG("��鴫����: " + sensor->getName(), sensor->getId());
            bool triggerAC = false;
            std::string reason;

            if (sensor->getTemperature() > tempThreshold) {
                triggerAC = true;
                reason = "�¶� (" + std::to_string(sensor->getTemperature()) + "C) ������ֵ (" + std::to_string(tempThreshold) + "C)";
                gLogger.INFO("������ " + sensor->getName() + ": " + reason, sensor->getId());
            }
            // Example: Add humidity check if needed
            // if (sensor->getHumidity() > humidityThreshold) {
            //     triggerAC = true; // Could be OR or AND logic with temperature
            //     reason += (reason.empty() ? "" : " �� ") + "ʪ�� (" + std::to_string(sensor->getHumidity()) + "%) ������ֵ (" + std::to_string(humidityThreshold) + "%)";
            //     gLogger.INFO("������ " + sensor->getName() + ": " + reason, sensor->getId());
            // }


            if (triggerAC) {
                // Find an AC and turn it on to cool
                for (Device* acDevice : devices) {
                    AC* ac = dynamic_cast<AC*>(acDevice);
                    if (ac) {
                        if (ac->getMode() != ACMode::COOL || !ac->isEmergencyPowerOff()) { // Check if not already cooling or powered off
                            gLogger.INFO("�����յ� '" + ac->getName() + "' �������䣬��Ϊ: " + reason, ac->getId());
                            ac->setMode(ACMode::COOL);
                            ac->setTargetTemperature(22.0); // Example target temp
                            ac->setFanSpeed(FanSpeed::AUTO);
                            ac->updateStatus(); // Display status
                            actionTaken = true;
							std::cout << "�յ� '" + ac->getName() + "' �ѿ������䡣" << std::endl;
                            // break; // Control one AC or all? For now, control the first one found.
                        }
                        else {
							std::cout << "�յ� '" + ac->getName() + "' �Ѵ�������ģʽ������ϵ��С�" << std::endl;
                            gLogger.INFO("�յ� '" + ac->getName() + "' �Ѵ�������ģʽ������ϵ��С�", ac->getId());
                        }
                    }
                }
            }
        }
    }
    if (!actionTaken) {
        std::cout<< "��ʪ�ȹ�������ϣ�δ�����κοյ�������" << std::endl;
        gLogger.INFO("��ʪ�ȹ�������ϣ�δ�����κοյ�������", -1);
    }
}

void User::runFireEmergencyRule(double co2FireThreshold) {
    std::cout << "����ģ��: �����ֽ������� (CO2��ֵ > " + std::to_string(co2FireThreshold * 100) + "%)." << std::endl;
    std::cout << "��־�ļ������������������־�ļ�" << std::endl;
    gLogger.ALERT("����ģ��: �����ֽ������� (CO2��ֵ > " + std::to_string(co2FireThreshold * 100) + "%).", -1);
    bool fireDetected = false;
    int detectingSensorId = -1;
    std::string sensorName;

    for (Device* device : devices) {
        Sensor* sensor = dynamic_cast<Sensor*>(device);
        if (sensor) {
            gLogger.DEBUG("�����ִ�����: " + sensor->getName() + ", CO2: " + std::to_string(sensor->getCO2Concentration() * 100) + "%", sensor->getId());
            if (sensor->getCO2Concentration() > co2FireThreshold) {
                fireDetected = true;
                detectingSensorId = sensor->getId();
                sensorName = sensor->getName();
                gLogger.ALERT("*** ���־���! *** ������ '" + sensorName + "' (ID: " + std::to_string(detectingSensorId) +
                    ") ��⵽CO2Ũ�� (" + std::to_string(sensor->getCO2Concentration() * 100) + "%) ������ֵ!", detectingSensorId);
                std::cout << "*** ���־���! *** ������ '" + sensorName + "' (ID: " + std::to_string(detectingSensorId) +
                    ") ��⵽CO2Ũ�� (" + std::to_string(sensor->getCO2Concentration() * 100) + "%) ������ֵ!", detectingSensorId,"\n";
                break;
            }
        }
    }

    if (fireDetected) {
        std::cout << "����ȷ��! ִ��ȫ�ݽ����ϵ�..." << std::endl;
        gLogger.ALERT("����ȷ��! ִ��ȫ�ݽ����ϵ�...", -1);
        int devicesAffected = 0;
        for (Device* device : devices) {
            if (!device->isEmergencyPowerOff()) { // Check if not already off
                device->setEmergencyPowerOff(true); // Set the flag
                // Specific actions for different devices on power off
                if (Light* light = dynamic_cast<Light*>(device)) {
                    light->turnOff(); // Ensure light is visually off
                }
                else if (AC* ac = dynamic_cast<AC*>(device)) {
                    ac->setMode(ACMode::OFF); // Turn AC off
                }
                // Other devices might just have the flag set.
                gLogger.INFO("�豸 '" + device->getName() + "' (ID: " + std::to_string(device->getId()) + ") �ѽ����ϵ硣", device->getId());
                devicesAffected++;
            }
        }
        gLogger.ALERT("ȫ�ݽ����ϵ���ɡ���Ӱ�� " + std::to_string(devicesAffected) + " ���豸����������ɢ!", -1);
        std::cout << "ȫ�ݽ����ϵ���ɡ���Ӱ�� " + std::to_string(devicesAffected) + " ���豸����������ɢ!" << std::endl;
    }
    else {
        std::cout << "���ֽ�����������ϣ�δ��⵽���֡�" << std::endl;
        gLogger.INFO("���ֽ�����������ϣ�δ��⵽���֡�", -1);
    }
}