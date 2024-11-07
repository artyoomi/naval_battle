#ifndef NO_SKILL_EXCEPTION_H
#define NO_SKILL_EXCEPTION_H

#include <stdexcept>

class NoSkillException : public std::exception
{
public:
    NoSkillException(const std::string &message) : _message{message} {}

    const char* what() const noexcept override { return _message.c_str(); }

private:
    std::string _message;
};

#endif
