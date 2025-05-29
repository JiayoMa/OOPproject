#include "SmartLogger.h"

// --- Helper function to get current timestamp ---
std::string SmartLogger::getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    // ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X"); // Using localtime

    // Using gmtime to avoid potential issues with std::localtime and thread-safety if used across threads later
    // However, std::gmtime also has thread-safety concerns with its static buffer on some platforms.
    // C++20 introduces thread-safe time formatting. For now, this is a common approach.
    std::tm timeinfo_tm;
#ifdef _WIN32
    gmtime_s(&timeinfo_tm, &in_time_t); // Windows specific
#else
    gmtime_r(&in_time_t, &timeinfo_tm); // POSIX specific
#endif
    ss << std::put_time(&timeinfo_tm, "%Y-%m-%d %H:%M:%S UTC");

    return ss.str();
}

std::string SmartLogger::levelToString(LogLevel level) {
    switch (level) {
    case LogLevel::DEBUG: return "DEBUG";
    case LogLevel::INFO:  return "INFO";
    case LogLevel::ALERT: return "ALERT";
    default: return "UNKNOWN";
    }
}

// --- ConsoleLogger Implementation ---
void ConsoleLogger::log(const std::string& message, LogLevel level, int deviceId, std::thread::id threadId) {
    // SmartLogger handles timestamp and level string, ConsoleLogger just prints
    std::cout << message << std::endl;
}

// --- FileLogger Implementation ---
FileLogger::FileLogger(const std::string& filename) {
    logFile.open(filename, std::ios::app); // Append mode
    if (!logFile.is_open()) {
        std::cerr << "错误: 无法打开日志文件 '" << filename << "'" << std::endl;
    }
}

FileLogger::~FileLogger() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

void FileLogger::log(const std::string& message, LogLevel level, int deviceId, std::thread::id threadId) {
    if (logFile.is_open()) {
        logFile << message << std::endl;
    }
}

// --- SmartLogger Implementation ---
SmartLogger::SmartLogger(LoggerStrategy* initialStrategy, LogLevel minLevel)
    : strategy(initialStrategy), minimumLevel(minLevel) {
}

SmartLogger::~SmartLogger() {
    delete strategy; // SmartLogger owns the strategy object passed to it
}

void SmartLogger::setStrategy(LoggerStrategy* newStrategy) {
    delete strategy; // Delete old strategy
    strategy = newStrategy;
}

void SmartLogger::setMinimumLevel(LogLevel level) {
    minimumLevel = level;
}

void SmartLogger::log(const std::string& message, LogLevel level, int deviceId) {
    if (level < minimumLevel) {
        return;
    }
    if (!strategy) {
        std::cerr << "错误: SmartLogger没有设置输出策略!" << std::endl;
        return;
    }

    std::stringstream formattedMessage;
    formattedMessage << "[" << getCurrentTimestamp() << "] ";
    formattedMessage << "[" << levelToString(level) << "] ";

    // Get thread ID (basic, more advanced might involve mapping to names)
    // std::stringstream tid_ss;
    // tid_ss << std::this_thread::get_id();
    // formattedMessage << "[Thread:" << tid_ss.str() << "] ";


    if (deviceId != -1) {
        formattedMessage << "[设备ID:" << deviceId << "] ";
    }
    formattedMessage << message;

    strategy->log(formattedMessage.str(), level, deviceId); // Pass all info to strategy if it needs it
}

void SmartLogger::DEBUG(const std::string& message, int deviceId) {
    log(message, LogLevel::DEBUG, deviceId);
}
void SmartLogger::INFO(const std::string& message, int deviceId) {
    log(message, LogLevel::INFO, deviceId);
}
void SmartLogger::ALERT(const std::string& message, int deviceId) {
    log(message, LogLevel::ALERT, deviceId);
}