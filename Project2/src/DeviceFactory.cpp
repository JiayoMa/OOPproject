#include "DeviceFactory.h" // This includes Sensor.h, Light.h, AC.h via Device.h if structured well, or include them directly
#include <iostream>

// --- SensorFactory Implementation ---
Device* SensorFactory::createDevice() {
    // Create a default Sensor, perhaps using DeviceParams default constructor
    DeviceParams defaultParams;
    return new Sensor(defaultParams.id, defaultParams.name, defaultParams.importance, defaultParams.powerConsumption,
                      defaultParams.temperature, defaultParams.humidity, defaultParams.co2Concentration);
}

Device* SensorFactory::createDeviceWithParams(const DeviceParams& params) {
    // Directly use the members of the params struct
    return new Sensor(params.id, params.name, params.importance, params.powerConsumption,
                      params.temperature, params.humidity, params.co2Concentration);
}

// --- LightFactory Implementation ---
Device* LightFactory::createDevice() {
    DeviceParams defaultParams;
    return new Light(defaultParams.id, defaultParams.name, defaultParams.importance, defaultParams.powerConsumption,
                     defaultParams.isOn, defaultParams.brightness);
}

Device* LightFactory::createDeviceWithParams(const DeviceParams& params) {
    return new Light(params.id, params.name, params.importance, params.powerConsumption,
                     params.isOn, params.brightness);
}

// --- ACFactory Implementation ---
Device* ACFactory::createDevice() {
    DeviceParams defaultParams;
    return new AC(defaultParams.id, defaultParams.name, defaultParams.importance, defaultParams.powerConsumption,
                  defaultParams.acMode, defaultParams.targetTemperature, defaultParams.fanSpeed);
}

Device* ACFactory::createDeviceWithParams(const DeviceParams& params) {
    return new AC(params.id, params.name, params.importance, params.powerConsumption,
                  params.acMode, params.targetTemperature, params.fanSpeed);
}