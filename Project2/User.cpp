#include "User.h"
#include <iostream> // For direct cout if logger not fully integrated everywhere

User::User(const std::string& name, std::vector<Device*>& deviceList)
    : username(name), devices(deviceList) {
    gLogger.INFO("用户 '" + username + "' 已初始化。");
}

std::string User::getUsername() const {
    return username;
}

void User::runTemperatureHumidityRule(double tempThreshold, double humidityThreshold) {
    std::cout << "场景模拟: 检查温湿度规则。" << std::endl;
    std::cout << "日志文件已输出，详情请检查日志文件" << std::endl;
    gLogger.INFO("场景模拟: 检查温湿度规则。", -1);
    bool actionTaken = false;

    for (Device* device : devices) {
        Sensor* sensor = dynamic_cast<Sensor*>(device);
        if (sensor) {
            gLogger.DEBUG("检查传感器: " + sensor->getName(), sensor->getId());
            bool triggerAC = false;
            std::string reason;

            if (sensor->getTemperature() > tempThreshold) {
                triggerAC = true;
                reason = "温度 (" + std::to_string(sensor->getTemperature()) + "C) 超出阈值 (" + std::to_string(tempThreshold) + "C)";
                gLogger.INFO("传感器 " + sensor->getName() + ": " + reason, sensor->getId());
            }
            // Example: Add humidity check if needed
            // if (sensor->getHumidity() > humidityThreshold) {
            //     triggerAC = true; // Could be OR or AND logic with temperature
            //     reason += (reason.empty() ? "" : " 且 ") + "湿度 (" + std::to_string(sensor->getHumidity()) + "%) 超出阈值 (" + std::to_string(humidityThreshold) + "%)";
            //     gLogger.INFO("传感器 " + sensor->getName() + ": " + reason, sensor->getId());
            // }


            if (triggerAC) {
                // Find an AC and turn it on to cool
                for (Device* acDevice : devices) {
                    AC* ac = dynamic_cast<AC*>(acDevice);
                    if (ac) {
                        if (ac->getMode() != ACMode::COOL || !ac->isEmergencyPowerOff()) { // Check if not already cooling or powered off
                            gLogger.INFO("触发空调 '" + ac->getName() + "' 开启制冷，因为: " + reason, ac->getId());
                            ac->setMode(ACMode::COOL);
                            ac->setTargetTemperature(22.0); // Example target temp
                            ac->setFanSpeed(FanSpeed::AUTO);
                            ac->updateStatus(); // Display status
                            actionTaken = true;
							std::cout << "空调 '" + ac->getName() + "' 已开启制冷。" << std::endl;
                            // break; // Control one AC or all? For now, control the first one found.
                        }
                        else {
							std::cout << "空调 '" + ac->getName() + "' 已处于制冷模式或紧急断电中。" << std::endl;
                            gLogger.INFO("空调 '" + ac->getName() + "' 已处于制冷模式或紧急断电中。", ac->getId());
                        }
                    }
                }
            }
        }
    }
    if (!actionTaken) {
        std::cout<< "温湿度规则检查完毕，未触发任何空调操作。" << std::endl;
        gLogger.INFO("温湿度规则检查完毕，未触发任何空调操作。", -1);
    }
}

void User::runFireEmergencyRule(double co2FireThreshold) {
    std::cout << "场景模拟: 检查火灾紧急规则 (CO2阈值 > " + std::to_string(co2FireThreshold * 100) + "%)." << std::endl;
    std::cout << "日志文件已输出，详情请检查日志文件" << std::endl;
    gLogger.ALERT("场景模拟: 检查火灾紧急规则 (CO2阈值 > " + std::to_string(co2FireThreshold * 100) + "%).", -1);
    bool fireDetected = false;
    int detectingSensorId = -1;
    std::string sensorName;

    for (Device* device : devices) {
        Sensor* sensor = dynamic_cast<Sensor*>(device);
        if (sensor) {
            gLogger.DEBUG("检查火灾传感器: " + sensor->getName() + ", CO2: " + std::to_string(sensor->getCO2Concentration() * 100) + "%", sensor->getId());
            if (sensor->getCO2Concentration() > co2FireThreshold) {
                fireDetected = true;
                detectingSensorId = sensor->getId();
                sensorName = sensor->getName();
                gLogger.ALERT("*** 火灾警报! *** 传感器 '" + sensorName + "' (ID: " + std::to_string(detectingSensorId) +
                    ") 检测到CO2浓度 (" + std::to_string(sensor->getCO2Concentration() * 100) + "%) 超出阈值!", detectingSensorId);
                std::cout << "*** 火灾警报! *** 传感器 '" + sensorName + "' (ID: " + std::to_string(detectingSensorId) +
                    ") 检测到CO2浓度 (" + std::to_string(sensor->getCO2Concentration() * 100) + "%) 超出阈值!", detectingSensorId,"\n";
                break;
            }
        }
    }

    if (fireDetected) {
        std::cout << "火灾确认! 执行全屋紧急断电..." << std::endl;
        gLogger.ALERT("火灾确认! 执行全屋紧急断电...", -1);
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
                gLogger.INFO("设备 '" + device->getName() + "' (ID: " + std::to_string(device->getId()) + ") 已紧急断电。", device->getId());
                devicesAffected++;
            }
        }
        gLogger.ALERT("全屋紧急断电完成。共影响 " + std::to_string(devicesAffected) + " 个设备。请立即疏散!", -1);
        std::cout << "全屋紧急断电完成。共影响 " + std::to_string(devicesAffected) + " 个设备。请立即疏散!" << std::endl;
    }
    else {
        std::cout << "火灾紧急规则检查完毕，未检测到火灾。" << std::endl;
        gLogger.INFO("火灾紧急规则检查完毕，未检测到火灾。", -1);
    }
}