#ifndef COMPRESSIONEXCEPTION_H
#define COMPRESSIONEXCEPTION_H

#include <stdexcept>
#include <string>

class CompressionException : public std::runtime_error {
public:
    explicit CompressionException(const std::string& message)
        : std::runtime_error(message) {}
};

#endif 