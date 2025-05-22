#include "UserManager.h"
#include <fstream>
#include <sstream>
#include <iostream>   
#include <algorithm>  

// extern SmartLogger gLogger; 

UserManager::UserManager(const std::string& filePath, std::vector<Device*>& devices)
    : usersFilePath(filePath), allDevices(devices) {
    loadUsersFromFile();
    if (users.empty()) {
        std::cout << "User data not found. Creating default admin (admin/admin)..." << std::endl;
        registerUser("admin", "admin", UserRole::ADMIN);
    }
}

// Destructor is not strictly needed as unique_ptr will clean up, 
// but if you had other raw pointers to manage, you would add it.
// UserManager::~UserManager() {}

void UserManager::loadUsersFromFile() {
    std::ifstream inFile(usersFilePath);
    if (!inFile.is_open()) {
        return;
    }

    users.clear(); // Clear existing unique_ptrs, freeing memory
    std::string line;
    while (std::getline(inFile, line)) {
        std::stringstream ss(line);
        std::string username, passwordHash, roleStr;

        if (std::getline(ss, username, ':') &&
            std::getline(ss, passwordHash, ':') &&
            std::getline(ss, roleStr)) {
            // Create User on the heap and store in unique_ptr
            users.push_back(std::make_unique<User>(username, passwordHash, stringToRole(roleStr), allDevices));
        }
        else {
            // std::cerr << "Warning: Malformed line in user file: " << line << std::endl;
        }
    }
    inFile.close();
}

void UserManager::saveUsersToFile() const {
    std::ofstream outFile(usersFilePath);
    if (!outFile.is_open()) {
        std::cerr << "Error: Cannot open user file '" << usersFilePath << "' for writing." << std::endl;
        return;
    }

    for (const auto& user_ptr : users) { // Iterate over unique_ptrs
        if (user_ptr) { // Check if the pointer is not null
            outFile << user_ptr->getUsername() << ":"
                << user_ptr->getPassword() << ":"
                << roleToString(user_ptr->getRole()) << std::endl;
        }
    }
    outFile.close();
}

User* UserManager::registerUser(const std::string& username, const std::string& password, UserRole role) {
    if (username.empty() || password.empty()) {
        std::cout << "Username and password cannot be empty." << std::endl;
        return nullptr;
    }
    for (const auto& user_ptr : users) {
        if (user_ptr && user_ptr->getUsername() == username) {
            std::cout << "Username '" << username << "' already exists." << std::endl;
            return nullptr;
        }
    }

    users.push_back(std::make_unique<User>(username, password, role, allDevices));
    saveUsersToFile();
    std::cout << "User '" << username << "' registered successfully." << std::endl;
    return users.back().get(); // Return raw pointer from the new unique_ptr
}

User* UserManager::loginUser(const std::string& username, const std::string& password) {
    for (const auto& user_ptr : users) {
        if (user_ptr && user_ptr->getUsername() == username && user_ptr->getPassword() == password) {
            std::cout << "User '" << username << "' logged in successfully." << std::endl;
            return user_ptr.get(); // Return raw pointer
        }
    }
    std::cout << "Login failed: Invalid username or password." << std::endl;
    return nullptr;
}

bool UserManager::deleteUser(const std::string& usernameToDelete, const User& adminUser) {
    if (adminUser.getRole() != UserRole::ADMIN) {
        std::cout << "Permission denied. Only admins can delete users." << std::endl;
        return false;
    }

    if (adminUser.getUsername() == usernameToDelete) {
        std::cout << "Admin cannot delete themselves." << std::endl;
        return false;
    }

    auto it = std::remove_if(users.begin(), users.end(),
        [&](const std::unique_ptr<User>& u_ptr) {
            return u_ptr && u_ptr->getUsername() == usernameToDelete;
        });

    if (it != users.end()) {
        users.erase(it, users.end()); // This erases the unique_ptrs, deleting the User objects
        saveUsersToFile();
        std::cout << "User '" << usernameToDelete << "' has been deleted." << std::endl;
        return true;
    }
    else {
        std::cout << "User '" << usernameToDelete << "' not found." << std::endl;
        return false;
    }
}

User* UserManager::findUser(const std::string& username) {
    for (const auto& user_ptr : users) {
        if (user_ptr && user_ptr->getUsername() == username) {
            return user_ptr.get(); // Return raw pointer
        }
    }
    return nullptr;
}