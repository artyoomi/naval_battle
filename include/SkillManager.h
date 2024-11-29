#ifndef SKILL_MANAGER_H
#define SKILL_MANAGER_H

#include <cstdint>

#include <deque>
#include <memory>

#include "skills/Skill.h"
#include "GameField.h"

/*
 * Передавать необходимые аргументы в конструкторе менеджера
 */

class SkillManager {
public:
    SkillManager(GameField& field, ShipManager& manager) noexcept;
    SkillManager& operator = (SkillManager&& other);

    std::string get_top_name() const;
    
    void add_skill(std::string name);
    void add_random_skill() noexcept;
    bool apply(std::size_t x, std::size_t y);

    void clear();

    json to_json() const;
    void from_json(const json &j);

    // debug function
    void show() const noexcept;

private:
    GameField&   _field;
    ShipManager& _manager;
    std::deque<std::unique_ptr<ISkill>> _skills;
};

#endif
