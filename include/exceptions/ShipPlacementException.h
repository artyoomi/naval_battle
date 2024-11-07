#ifndef SHIP_PLACEMENT_EXCEPTION_H
#define SHIP_PLACEMENT_EXCEPTION_H

#include <stdexcept>

class ShipPlacementException : public std::exception
{
public:
    ShipPlacementException(const std::string& message) : _message{message} {}

    const char* what() const noexcept override { return _message.c_str(); }

private:
    std::string _message;
};

#endif
