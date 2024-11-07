#ifndef NO_SKILLS_EXCEPTION_H
#define NO_SKILLS_EXCEPTION_H

#include <stdexcept>

class NoSkillsException : public std::exception
{
public:
    NoSkillsException(const std::string &message) : _message{message} {}

    const char* what() const noexcept override { return _message.c_str(); }

private:
    std::string _message;
};

#endif
