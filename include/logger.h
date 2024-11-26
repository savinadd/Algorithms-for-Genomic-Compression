// Logger.h

#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <mutex>

class Logger {
public:
    static Logger& getInstance();

    void log(const std::string& message);

private:
    Logger() = default;
    std::mutex logMutex;
};

#endif
