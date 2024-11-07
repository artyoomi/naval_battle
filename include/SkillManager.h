#ifndef SKILL_MANAGER_H
#define SKILL_MANAGER_H

#include <cstdint>

#include <deque>
#include <memory>

#include "skills/Skill.h"
#include "GameField.h"

class SkillManager {
public:
    SkillManager() noexcept;

    void add_random_skill() noexcept;
    void apply(GameField& field, ShipManager& manager, std::size_t x, std::size_t y);

    // debug function
    void show() const noexcept;

private:
    std::deque<std::unique_ptr<ISkill>> _skills;
};

#endif
