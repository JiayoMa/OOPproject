#pragma once
#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <vector>
#include <string>
#include <memory> // For std::unique_ptr
#include "User.h" 

class UserManager {
private:
    std::vector<std::unique_ptr<User>> users; // Store unique_ptrs to User
    std::string usersFilePath;
    std::vector<Device*>& allDevices;

    void loadUsersFromFile();
    void saveUsersToFile() const;

public:
    UserManager(const std::string& filePath, std::vector<Device*>& devices);
    // No explicit ~UserManager needed if unique_ptr handles memory

    // Return raw pointers (User*) for convenience, but ownership remains with unique_ptr
    User* registerUser(const std::string& username, const std::string& password, UserRole role = UserRole::USER);
    User* loginUser(const std::string& username, const std::string& password);

    bool deleteUser(const std::string& usernameToDelete, const User& adminUser);
    User* findUser(const std::string& username); // Returns raw pointer
};

#endif // USERMANAGER_H