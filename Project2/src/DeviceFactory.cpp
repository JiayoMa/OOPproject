#include "../include/DeviceFactory.h"
#include "../include/Sensor.h" // Ensure derived classes are included
#include "../include/Light.h"
#include "../include/AC.h"


// SensorFactory Implementation
Device* SensorFactory::createDevice() {
    // 默认传感器参数
    return new Sensor(0, "默认传感器", DeviceImportance::MEDIUM, 0.1, 20.0, 50.0, 0.0004);
}

Device* SensorFactory::createDeviceWithParams(const DeviceParams& params) {
    return new Sensor(params.id, params.name, params.importance, params.powerConsumption,
        params.temperature, params.humidity, params.co2Concentration);
}

// LightFactory Implementation
Device* LightFactory::createDevice() {
    // 默认灯具参数
    return new Light(0, "默认灯具", DeviceImportance::LOW, 0.05, false, 50);
}

Device* LightFactory::createDeviceWithParams(const DeviceParams& params) {
    return new Light(params.id, params.name, params.importance, params.powerConsumption,
        params.isOn, params.brightness);
}

// ACFactory Implementation
Device* ACFactory::createDevice() {
    // 默认空调参数
    return new AC(0, "默认空调", DeviceImportance::HIGH, 1.5, ACMode::OFF, 22.0, FanSpeed::AUTO);
}

Device* ACFactory::createDeviceWithParams(const DeviceParams& params) {
    return new AC(params.id, params.name, params.importance, params.powerConsumption,
        params.acMode, params.targetTemperature, params.fanSpeed);
}