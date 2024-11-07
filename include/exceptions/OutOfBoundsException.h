#ifndef OUT_OF_BOUNDS_EXCEPTION_H
#define OUT_OF_BOUNDS_EXCEPTION_H

#include <stdexcept>

class OutOfBoundsException : public std::exception
{
public:
    OutOfBoundsException(const std::string& message) : _message{message} {}

    const char* what() const noexcept override { return  _message.c_str(); }

private:
    std::string _message;
};

#endif
