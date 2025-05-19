#pragma once // Use #pragma once or standard include guards
#ifndef SMARTLOGGER_H
#define SMARTLOGGER_H

#include <string>
#include <fstream>
#include <iostream>
#include <chrono>
#include <iomanip> // For std::put_time
#include <sstream> // For formatting time
#include <thread>  // For std::this_thread::get_id() later

enum class LogLevel {
    DEBUG,
    INFO,
    ALERT
};

// Abstract Logger class (Interface)
class LoggerStrategy {
public:
    virtual ~LoggerStrategy() = default;
    virtual void log(const std::string& message, LogLevel level, int deviceId = -1, std::thread::id threadId = std::this_thread::get_id()) = 0;
};

// Concrete Logger: Console
class ConsoleLogger : public LoggerStrategy {
public:
    void log(const std::string& message, LogLevel level, int deviceId = -1, std::thread::id threadId = std::this_thread::get_id()) override;
};

// Concrete Logger: File
class FileLogger : public LoggerStrategy {
private:
    std::ofstream logFile;
public:
    FileLogger(const std::string& filename);
    ~FileLogger() override;
    void log(const std::string& message, LogLevel level, int deviceId = -1, std::thread::id threadId = std::this_thread::get_id()) override;
};

// SmartLogger class (Context)
class SmartLogger {
private:
    LoggerStrategy* strategy; // Using a single strategy for now, can be a list for multiple outputs
    LogLevel minimumLevel;

    std::string levelToString(LogLevel level);
    std::string getCurrentTimestamp();

public:
    SmartLogger(LoggerStrategy* strategy, LogLevel minLevel = LogLevel::INFO);
    ~SmartLogger();

    void setStrategy(LoggerStrategy* newStrategy); // Allow changing strategy
    void setMinimumLevel(LogLevel level);

    void log(const std::string& message, LogLevel level, int deviceId = -1);

    // Simplified macro-like functions (true macros are preprocessor directives)
    void DEBUG(const std::string& message, int deviceId = -1);
    void INFO(const std::string& message, int deviceId = -1);
    void ALERT(const std::string& message, int deviceId = -1);
};

// Macro definitions (optional, but as requested)
// These would typically be outside the class, in the .h file
// For simplicity, we'll use member functions above that act like macros.
// True macros would look like:
// #define LOG_ALERT(logger_instance, msg, devId) logger_instance.log(msg, LogLevel::ALERT, devId)

#endif // SMARTLOGGER_H