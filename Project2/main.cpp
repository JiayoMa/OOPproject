#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <algorithm>
#include <fstream> // Required for file operations
#include <sstream> // Required for parsing lines

#include "Device.h"
#include "Sensor.h"
#include "Light.h"
#include "AC.h"
#include "DeviceFactory.h"
#include "User.h"      // Forward declare or include if User is defined
#include "SmartLogger.h" // Include for SmartLogger, FileLogger, LogLevel definitions

// --- Global SmartLogger Instance (will be initialized later) ---
// SmartLogger logger; // Or a pointer, to be initialized in main

// --- FORWARD DECLARATIONS FOR USER ---
class User; // Forward declaration
// extern SmartLogger gLogger; // Declare gLogger as extern. Definition will be provided below in this file.
// The line above is kept to show it's an extern declaration, but its definition is in this file.
// For a global defined and used in the same file, 'extern' is not strictly needed before definition.
// However, if other files were to include a header declaring 'extern SmartLogger gLogger;',
// then this .cpp file would provide the one definition.

// --- Global Factories (already defined) ---
SensorFactory sensorFactory;
LightFactory lightFactory;
ACFactory acFactory;


// --- HELPER FUNCTION to convert string to DeviceImportance ---
DeviceImportance stringToImportance(const std::string& s) {
    int val = std::stoi(s);
    if (val == 0) return DeviceImportance::LOW;
    if (val == 1) return DeviceImportance::MEDIUM;
    if (val == 2) return DeviceImportance::HIGH;
    if (val == 3) return DeviceImportance::CRITICAL;
    return DeviceImportance::MEDIUM; // Default
}

std::string importanceToString(DeviceImportance imp) {
    switch (imp) {
    case DeviceImportance::LOW: return "0";
    case DeviceImportance::MEDIUM: return "1";
    case DeviceImportance::HIGH: return "2";
    case DeviceImportance::CRITICAL: return "3";
    default: return "1";
    }
}

// --- HELPER FUNCTION to convert string to ACMode ---
ACMode stringToACMode(const std::string& s) {
    int val = std::stoi(s);
    if (val >= 0 && val <= 3) return static_cast<ACMode>(val);
    return ACMode::OFF; // Default
}
std::string acModeToString(ACMode mode) {
    return std::to_string(static_cast<int>(mode));
}

// --- HELPER FUNCTION to convert string to FanSpeed ---
FanSpeed stringToFanSpeed(const std::string& s) {
    int val = std::stoi(s);
    if (val >= 0 && val <= 3) return static_cast<FanSpeed>(val);
    return FanSpeed::AUTO; // Default
}
std::string fanSpeedToString(FanSpeed speed) {
    return std::to_string(static_cast<int>(speed));
}


// --- MODIFIED: importDevicesFromFile ---
void importDevicesFromFile(std::vector<Device*>& devices, const std::string& filename = "devices.txt") {
    std::ifstream inFile(filename);
    if (!inFile.is_open()) {
        std::cerr << "错误: 无法打开设备文件 '" << filename << "' 进行读取。" << std::endl;
        std::cerr << "请注意文件写入规范：TYPE,ID,Name,Importance(0-3),PowerConsumption,emergencyPowerOff(0/1),[specific_params...]" << std::endl;
        //TYPE,ID,Name,Importance(0-3),PowerConsumption,emergencyPowerOff(0/1),[specific_params...]
        // gLogger.log("无法打开设备文件进行读取: " + filename, LogLevel::ALERT); // Example with logger
        return;
    }

    std::string line;
    while (std::getline(inFile, line)) {
        if (line.empty() || line[0] == '#') continue; // Skip empty lines or comments

        std::stringstream ss(line);
        std::string segment;
        std::vector<std::string> segments;
        while (std::getline(ss, segment, ',')) {
            segments.push_back(segment);
        }

        if (segments.empty()) continue;

        std::string typeStr = segments[0];
        try {
            DeviceParams params;
            params.id = std::stoi(segments[1]);

            // Check for duplicate ID before adding
            bool id_exists = false;
            for (const auto& dev : devices) {
                if (dev->getId() == params.id) {
                    id_exists = true;
                    break;
                }
            }
            if (id_exists) {
                std::cerr << "警告: 文件中设备ID " << params.id << " 已存在于当前设备列表，跳过导入。" << std::endl;
                continue;
            }

            params.name = segments[2];
            params.importance = stringToImportance(segments[3]);
            params.powerConsumption = std::stod(segments[4]);
            bool emergencyOff = std::stoi(segments[5]) == 1;


            Device* newDevice = nullptr;
            if (typeStr == "SENSOR" && segments.size() >= 9) {
                params.temperature = std::stod(segments[6]);
                params.humidity = std::stod(segments[7]);
                params.co2Concentration = std::stod(segments[8]);
                newDevice = sensorFactory.createDeviceWithParams(params);
            }
            else if (typeStr == "LIGHT" && segments.size() >= 8) {
                params.isOn = (std::stoi(segments[6]) == 1);
                params.brightness = std::stoi(segments[7]);
                newDevice = lightFactory.createDeviceWithParams(params);
            }
            else if (typeStr == "AC" && segments.size() >= 9) {
                params.acMode = stringToACMode(segments[6]);
                params.targetTemperature = std::stod(segments[7]);
                params.fanSpeed = stringToFanSpeed(segments[8]);
                newDevice = acFactory.createDeviceWithParams(params);
            }
            else {
                std::cerr << "警告: 文件中设备类型 '" << typeStr << "'无法识别或参数不足，行: " << line << std::endl;
                continue;
            }

            if (newDevice) {
                newDevice->setEmergencyPowerOff(emergencyOff); // Set this explicitly
                devices.push_back(newDevice);
            }
        }
        catch (const std::invalid_argument& e) {
            std::cerr << "错误: 解析行时发生无效参数错误 '" << line << "': " << e.what() << std::endl;
        }
        catch (const std::out_of_range& e) {
            std::cerr << "错误: 解析行时发生越界错误 '" << line << "': " << e.what() << std::endl;
        }
        catch (const std::exception& e) {
            std::cerr << "错误: 解析行时发生未知错误 '" << line << "': " << e.what() << std::endl;
        }
    }
    inFile.close();
    std::cout << "设备已从 '" << filename << "' 导入完毕。" << std::endl;
    // gLogger.log("设备导入完成。", LogLevel::INFO);
}

// --- MODIFIED: saveDevicesToFile ---
void saveDevicesToFile(const std::vector<Device*>& devices, const std::string& filename = "data/devices.txt") {
    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        std::cerr << "错误: 无法打开设备文件 '" << filename << "' 进行写入。" << std::endl;
        // gLogger.log("无法打开设备文件进行写入: " + filename, LogLevel::ALERT);
        return;
    }

    outFile << "# TYPE,ID,Name,Importance(0-3),PowerConsumption,emergencyPowerOff(0/1),[specific_params...]\n";
    outFile << "# Sensor: temperature,humidity,co2Concentration\n";
    outFile << "# Light: isOn(0/1),brightness\n";
    outFile << "# AC: mode(0-3),targetTemperature,fanSpeed(0-3)\n";

    for (const auto& device : devices) {
        outFile << device->getId() << "," // ID first for easier manual checking, but type first is better for parsing
            << device->getName() << ","
            << importanceToString(device->getImportance()) << ","
            << device->getPowerConsumption() << ","
            << (device->isEmergencyPowerOff() ? "1" : "0");

        // Attempt to dynamically cast to get specific parameters
        if (const Sensor* sensor = dynamic_cast<const Sensor*>(device)) {
            outFile << ",SENSOR"; // Type identifier after common params for this save format
            outFile << "," << sensor->getTemperature()
                << "," << sensor->getHumidity()
                << "," << sensor->getCO2Concentration();
        }
        else if (const Light* light = dynamic_cast<const Light*>(device)) {
            outFile << ",LIGHT";
            outFile << "," << (light->getIsOn() ? "1" : "0")
                << "," << light->getBrightness();
        }
        else if (const AC* ac = dynamic_cast<const AC*>(device)) {
            outFile << ",AC";
            outFile << "," << acModeToString(ac->getMode())
                << "," << ac->getTargetTemperature()
                << "," << fanSpeedToString(ac->getFanSpeed());
        }
        outFile << std::endl;
    }
    // Re-arranging the save format slightly to put TYPE first for easier parsing
    // The previous saveDevicesToFile was a bit mixed up. Let's fix it.
    outFile.close(); // Close the current one
    outFile.open(filename, std::ofstream::out | std::ofstream::trunc); // Re-open to overwrite

    outFile << "# TYPE,ID,Name,Importance(0-3),PowerConsumption,emergencyPowerOff(0/1),[specific_params...]\n";
    outFile << "# Sensor: temperature,humidity,co2Concentration\n";
    outFile << "# Light: isOn(0/1),brightness\n";
    outFile << "# AC: mode(0-3),targetTemperature,fanSpeed(0-3)\n";

    for (const auto& device : devices) {
        std::string typeStr;
        std::string specificParamsStr;

        if (const Sensor* sensor = dynamic_cast<const Sensor*>(device)) {
            typeStr = "SENSOR";
            specificParamsStr = "," + std::to_string(sensor->getTemperature()) +
                "," + std::to_string(sensor->getHumidity()) +
                "," + std::to_string(sensor->getCO2Concentration());
        }
        else if (const Light* light = dynamic_cast<const Light*>(device)) {
            typeStr = "LIGHT";
            specificParamsStr = "," + std::to_string(light->getIsOn() ? 1 : 0) +
                "," + std::to_string(light->getBrightness());
        }
        else if (const AC* ac = dynamic_cast<const AC*>(device)) {
            typeStr = "AC";
            specificParamsStr = "," + acModeToString(ac->getMode()) +
                "," + std::to_string(ac->getTargetTemperature()) +
                "," + fanSpeedToString(ac->getFanSpeed());
        }
        else {
            continue; // Unknown device type, skip
        }

        outFile << typeStr << ","
            << device->getId() << ","
            << device->getName() << ","
            << importanceToString(device->getImportance()) << ","
            << device->getPowerConsumption() << ","
            << (device->isEmergencyPowerOff() ? "1" : "0")
            << specificParamsStr << std::endl;
    }


    outFile.close();
    std::cout << "所有设备信息已保存至 '" << filename << "'。" << std::endl;
    // gLogger.log("设备保存完成。", LogLevel::INFO);
}


void initializeAndPrintUser() {
    std::cout << "功能 '1 ---- 初始化并打印当前用户'" << std::endl;
    std::cout << "当前登录用户 (来自系统): " << "JiayoMa" << std::endl; // Placeholder for your username
    // Later: Create/load a User object and print its details.
    // gLogger.log("用户信息已打印。", LogLevel::INFO);
}

// findDeviceById and deleteDeviceById remain largely the same but could use logger
void findDeviceById(const std::vector<Device*>& devices) {
    if (devices.empty()) {
        std::cout << "设备列表为空，无法查找。" << std::endl;
        return;
    }
    std::cout << "请输入要查找的设备ID: ";
    int id;
    std::cin >> id;
    if (std::cin.fail()) {
        std::cout << "输入ID无效。" << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    bool found = false;
    for (const auto& device : devices) {
        if (device->getId() == id) {
            std::cout << "找到设备:" << std::endl;
            std::cout << *device << std::endl;
            // gLogger.log("找到设备 ID: " + std::to_string(id), LogLevel::INFO, id);
            found = true;
            break;
        }
    }
    if (!found) {
        std::cout << "未找到ID为 " << id << " 的设备。" << std::endl;
        // gLogger.log("未找到设备 ID: " + std::to_string(id), LogLevel::INFO);
    }
}

void deleteDeviceById(std::vector<Device*>& devices) {
    if (devices.empty()) {
        std::cout << "设备列表为空，无法删除。" << std::endl;
        return;
    }
    std::cout << "请输入要删除的设备ID: ";
    int id;
    std::cin >> id;
    if (std::cin.fail()) {
        std::cout << "输入ID无效。" << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    bool deleted = false;
    auto it = std::remove_if(devices.begin(), devices.end(),
        [&deleted, id](Device* device) {
            if (device->getId() == id) {
                delete device;
                deleted = true;
                return true;
            }
            return false;
        });

    if (deleted) {
        devices.erase(it, devices.end());
        std::cout << "ID为 " << id << " 的设备已删除。" << std::endl;
        // gLogger.log("设备 ID: " + std::to_string(id) + " 已删除。", LogLevel::INFO, id);
    }
    else {
        std::cout << "未找到ID为 " << id << " 的设备，无法删除。" << std::endl;
        // gLogger.log("尝试删除设备 ID: " + std::to_string(id) + " - 未找到。", LogLevel::INFO);
    }
}

// Forward declaration for User, to be defined in User.h and User.cpp
// class User; // Already forward declared

// --- MODIFIED: simulateSmartScenes ---
// We need a User object and the list of devices.
// For now, we'll create a dummy User object or pass devices directly.
void simulateSmartScenes(User& user, std::vector<Device*>& devices); // Declaration modified


void printMainMenu() {
    std::cout << "\n=========== 主菜单 ===========\n";
	std::cout << "0 ---- 创建新用户\n";
    std::cout << "1 ---- 初始化并打印当前用户\n";
    std::cout << "2 ---- 从文件导入设备\n";
    std::cout << "3 ---- 从键盘添加设备\n";
    std::cout << "4 ---- 列表显示当前所有设备\n";
    std::cout << "5 ---- 按指定ID查找设备\n";
    std::cout << "6 ---- 删除指定ID的设备\n";
    std::cout << "7 ---- 保存所有设备信息至文件中\n";
    std::cout << "8 ---- 智能场景模拟\n";
    std::cout << "Q ---- 退出\n";
    std::cout << "============================\n";
    std::cout << "请选择: ";
}


int main() {
    // --- Initialize SmartLogger (example) ---
    // The global gLogger is now defined at the end of this file.
    // If you needed a local logger or wanted to change gLogger's strategy:
    // FileLogger fileStrategy("main_specific.log");
    // ConsoleLogger consoleStrategy;
    // gLogger.setStrategy(&fileStrategy); // Example: Change strategy (make sure gLogger handles deletion correctly if it owns the strategy)
    // Or create a local logger:
    // SmartLogger localLogger(new ConsoleLogger(), LogLevel::DEBUG);
    // localLogger.INFO("Main function started.");

    // Create the "data" directory if it doesn't exist
    // system("mkdir data"); // For Linux/macOS
    // system("IF NOT EXIST data mkdir data"); // For Windows
    // Or use C++17 filesystem library if available and preferred

    std::vector<Device*> allDevices;
    User currentUser("JiayoMa", allDevices); // Create a User instance

    char choice_char;
    do {
        printMainMenu();
        std::cin >> choice_char;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        char lower_choice = std::tolower(choice_char);

        if (lower_choice == 'q') {
            // gLogger.log("系统退出。", LogLevel::INFO);
            std::cout << "正在退出智能家居系统。再见!" << std::endl;
            break;
        }

        int choice_int = -1;
        if (lower_choice >= '1' && lower_choice <= '8') {
            choice_int = lower_choice - '0';
        }
        else if (lower_choice != 'q') {
            std::cout << "无效的选择。请输入菜单中的数字或 'Q' 退出。" << std::endl;
            continue;
        }

        switch (choice_int) {
        case 0:
            break;
        case 1:
            initializeAndPrintUser(); // Will be enhanced with User object
            break;
        case 2:
            importDevicesFromFile(allDevices); // Pass the main device list
            break;
        case 3: {
            std::cout << "\n--- 从键盘添加设备 ---\n";
            std::cout << "选择设备类型 (1: 传感器, 2: 灯具, 3: 空调): ";
            int typeChoice;
            std::cin >> typeChoice;

            if (std::cin.fail() || typeChoice < 1 || typeChoice > 3) {
                std::cout << "无效的设备类型选择。" << std::endl;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            Device* newDevice = nullptr;
            DeviceParams currentParams;

            std::cout << "请输入设备ID: ";
            std::cin >> currentParams.id;
            if (std::cin.fail()) {
                std::cout << "ID输入无效。" << std::endl;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            bool id_exists = false;
            for (const auto& dev : allDevices) {
                if (dev->getId() == currentParams.id) {
                    id_exists = true;
                    break;
                }
            }
            if (id_exists) {
                std::cout << "错误：设备ID " << currentParams.id << " 已存在。请使用唯一的ID。" << std::endl;
                // gLogger.log("尝试添加重复ID设备: " + std::to_string(currentParams.id), LogLevel::ALERT);
                continue;
            }

            std::cout << "请输入设备名称: ";
            std::getline(std::cin, currentParams.name);
            std::cout << "请输入重要程度 (0:低, 1:中, 2:高, 3:危急): ";
            int impChoice;
            std::cin >> impChoice;
            currentParams.importance = stringToImportance(std::to_string(impChoice));
            std::cout << "请输入能耗: ";
            std::cin >> currentParams.powerConsumption;


            try {
                if (typeChoice == 1) {
                    std::cout << "请输入初始温度 (C): ";
                    std::cin >> currentParams.temperature;
                    std::cout << "请输入初始湿度 (%): ";
                    std::cin >> currentParams.humidity;
                    std::cout << "请输入初始CO2浓度 (例如, 0.04 代表 4%): ";
                    std::cin >> currentParams.co2Concentration;
                    if (std::cin.fail()) { throw std::runtime_error("传感器参数输入无效。"); }
                    newDevice = sensorFactory.createDeviceWithParams(currentParams);
                }
                else if (typeChoice == 2) {
                    std::cout << "灯具是否初始开启? (1 代表是, 0 代表否): ";
                    int onState;
                    std::cin >> onState;
                    currentParams.isOn = (onState == 1);
                    if (currentParams.isOn) {
                        std::cout << "请输入初始亮度 (0-100%): ";
                        std::cin >> currentParams.brightness;
                    }
                    else {
                        currentParams.brightness = 0;
                    }
                    if (std::cin.fail()) { throw std::runtime_error("灯具参数输入无效。"); }
                    newDevice = lightFactory.createDeviceWithParams(currentParams);
                }
                else if (typeChoice == 3) {
                    int modeChoice, fanChoice;
                    std::cout << "请输入空调模式 (0:关闭, 1:制冷, 2:制热, 3:送风): ";
                    std::cin >> modeChoice;
                    currentParams.acMode = static_cast<ACMode>(modeChoice);
                    if (currentParams.acMode != ACMode::OFF) {
                        std::cout << "请输入目标温度 (C): ";
                        std::cin >> currentParams.targetTemperature;
                        std::cout << "请输入风速 (0:低, 1:中, 2:高, 3:自动): ";
                        std::cin >> fanChoice;
                        currentParams.fanSpeed = static_cast<FanSpeed>(fanChoice);
                    }
                    if (std::cin.fail()) { throw std::runtime_error("空调参数输入无效。"); }
                    newDevice = acFactory.createDeviceWithParams(currentParams);
                }

                if (newDevice) {
                    allDevices.push_back(newDevice);
                    std::cout << "设备 '" << newDevice->getName() << "' (ID: " << newDevice->getId() << ") 创建成功。" << std::endl;
                    // gLogger.log("设备创建成功: " + newDevice->getName(), LogLevel::INFO, newDevice->getId());
                }
            }
            catch (const std::exception& e) {
                std::cerr << "创建设备时发生错误: " << e.what() << std::endl;
                // gLogger.log("创建设备失败: " + std::string(e.what()), LogLevel::ALERT);
                if (newDevice) {
                    delete newDevice;
                    newDevice = nullptr;
                }
                std::cin.clear();
            }
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            break;
        }
        case 4: {
            std::cout << "\n--- 当前所有设备列表 ---\n";
            if (allDevices.empty()) {
                std::cout << "尚未创建任何设备。" << std::endl;
            }
            else {
                for (const auto& device : allDevices) {
                    std::cout << *device << std::endl;
                }
            }
            break;
        }
        case 5:
            findDeviceById(allDevices);
            break;
        case 6:
            deleteDeviceById(allDevices);
            break;
        case 7:
            saveDevicesToFile(allDevices); // Pass the main device list
            break;
        case 8:
            simulateSmartScenes(currentUser, allDevices); // Pass User and devices
            break;
        }
    } while (true);

    for (Device* device : allDevices) {
        delete device;
    }
    allDevices.clear();

    return 0;
}

// Definition of simulateSmartScenes (will be moved to User.cpp or a SceneManager.cpp later)
// This is a placeholder, actual logic will be in User class methods.
void simulateSmartScenes(User& user, std::vector<Device*>& devices) {
    std::cout << "\n--- 智能场景模拟 ---" << std::endl;
    std::cout << "1. 模拟温湿度超限 (空调启动)" << std::endl;
    std::cout << "2. 模拟火灾检测 (全屋断电)" << std::endl;
    std::cout << "请选择要模拟的场景: ";
    int sceneChoice;
    std::cin >> sceneChoice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (sceneChoice == 1) {
        user.runTemperatureHumidityRule();
        std::cout<<std::endl;
        std::cout << "场景模拟结束。" << std::endl;
    }
    else if (sceneChoice == 2) {
        user.runFireEmergencyRule();
        std::cout << std::endl;
        std::cout << "场景模拟结束。" << std::endl;
    }
    else {
        std::cout << "无效的场景选择。" << std::endl;
    }
}

// Definition for the global logger instance
// This provides the actual definition for the 'extern SmartLogger gLogger;' declaration.
// It's crucial that this definition appears in exactly one .cpp file.
// The SmartLogger will own and delete the FileLogger instance upon its own destruction (e.g., at program exit).
SmartLogger gLogger(new FileLogger("smart_home_log.txt"), LogLevel::INFO);

// If gLogger were a pointer, the definition would be:
// SmartLogger* gLogger = nullptr;
// And then in main(): gLogger = new SmartLogger(new FileLogger("smart_home_log.txt"));
// And you'd have to manage its deletion: delete gLogger; before main exits.
// Using a global object instance as done above is simpler for cleanup.