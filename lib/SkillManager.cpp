#include "../include/SkillManager.h"

#include "../include/exceptions/NoSkillsException.h"

#include <iostream>     // for debug
#include <random>
#include <algorithm>
#include <memory>

#include "../include/skills/DoubleDamage.h"
#include "../include/skills/Scanner.h"
#include "../include/skills/Bombing.h"

SkillManager::SkillManager() noexcept
{
    _skills.emplace_back(std::make_unique<DoubleDamage>());
    _skills.emplace_back(std::make_unique<Scanner>());
    _skills.emplace_back(std::make_unique<Bombing>());

    std::random_device rd_dev;
    std::mt19937 rd_gen(rd_dev());
    std::shuffle(_skills.begin(), _skills.end(), rd_gen);
}

void SkillManager::add_random_skill() noexcept
{
    std::random_device rd_dev;
    std::mt19937 rd_gen(rd_dev());
    std::uniform_int_distribution<> rd_distr(0, 2);

    switch (rd_distr(rd_gen)) {
        case 0:
            _skills.emplace_back(std::make_unique<DoubleDamage>());
            break;
        case 1:
            _skills.emplace_back(std::make_unique<Scanner>());
            break;
        case 2:
            _skills.emplace_back(std::make_unique<Bombing>());
            break;
    }
}

void SkillManager::apply(GameField& field, ShipManager& manager, std::size_t x, std::size_t y)
{
    if (_skills.size() == 0)
        throw NoSkillsException("No skills to cast!");

    (_skills.front())->apply(field, manager, x, y);
    std::cout << "Skill " << (_skills.front())->name() << " was applied!\n";
    _skills.pop_front();
}

void SkillManager::show() const noexcept
{
    std::cout << "Current SkillManager abilities:\n";
    for (std::size_t i = 0; i < _skills.size(); ++i)
        std::cout << "-> " << _skills[i]->name() << "\n";
}
