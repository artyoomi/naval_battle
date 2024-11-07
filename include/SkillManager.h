#ifndef SKILL_MANAGER_H
#define SKILL_MANAGER_H

#include <cstdint>

#include <queue>

#include "skills/Skill.h"

class SkillManager {
public:
    enum Skill : uint8_t {DOUBLE_DAMAGE = 0, SCANNER = 1, BOMBING = 2};

    SkillManager() noexcept;

    void get_random_skill() noexcept;
    void apply();

private:
    std::deque<Skill> _skills;
};

#endif
