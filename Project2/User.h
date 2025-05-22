#pragma once
#ifndef USER_H
#define USER_H

#include <string>
#include <vector>
#include "Device.h" 
#include "SmartLogger.h" 

// extern SmartLogger gLogger; // Only if gLogger is truly global and defined elsewhere

enum class UserRole {
    USER,
    ADMIN
};

// Helper functions for UserRole (can be in .cpp if preferred)
inline std::string roleToString(UserRole role) {
    switch (role) {
    case UserRole::ADMIN: return "ADMIN";
    case UserRole::USER: return "USER";
    default: return "USER"; // Should ideally not happen
    }
}

inline UserRole stringToRole(const std::string& roleStr) {
    if (roleStr == "ADMIN") return UserRole::ADMIN;
    return UserRole::USER;
}

class User {
private:
    std::string username;
    std::string password;
    UserRole role;
    std::vector<Device*>& devices;

public:
    User(const std::string& name, const std::string& pwd, UserRole r, std::vector<Device*>& deviceList);

    std::string getUsername() const;
    std::string getPassword() const;
    UserRole getRole() const;
    const std::vector<Device*>& getDevices() const; // If User needs to expose this

    void runTemperatureHumidityRule(double tempThreshold = 30.0, double humidityThreshold = 70.0);
    void runFireEmergencyRule(double co2FireThreshold = 0.06);
};

#endif // USER_H