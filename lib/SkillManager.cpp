#include "../include/SkillManager.h"

#include "../include/exceptions/NoSkillsException.h"

#include <iostream>     // for debug
#include <random>
#include <algorithm>
#include <memory>

#include "../include/GameField.h"
#include "../include/ShipManager.h"
#include "../include/skills/DoubleDamage.h"
#include "../include/skills/Scanner.h"
#include "../include/skills/Bombing.h"

SkillManager::SkillManager(GameField& field, ShipManager& manager) noexcept : _field{field}, _manager{manager}
{   
    _skills.emplace_back(std::make_unique<DoubleDamage>());
    _skills.emplace_back(std::make_unique<Scanner>());
    _skills.emplace_back(std::make_unique<Bombing>());

    std::random_device rd_dev;
    std::mt19937 rd_gen(rd_dev());
    std::shuffle(_skills.begin(), _skills.end(), rd_gen);
}

SkillManager& SkillManager::operator = (SkillManager&& other)
{
    if (&other != this) {
        std::swap(_field, other._field);
        std::swap(_manager, other._manager);
        std::swap(_skills, other._skills);
    }

    return *this;
}

std::string SkillManager::get_top_name() const
{
    return _skills.front()->name();
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

bool SkillManager::apply(std::size_t x, std::size_t y)
{
    if (_skills.size() == 0)
        throw NoSkillsException("No skills to cast!");

    bool result = (_skills.front())->apply(_field, _manager, x, y);
    std::cout << "Skill " << (_skills.front())->name() << " was applied!\n";
    _skills.pop_front();
    
    return result;
}

json SkillManager::to_json() const
{
    json j;

    for (std::size_t i = 0; i < _skills.size(); ++i)
        j["skills"].push_back(_skills[i]->name());    

    return j;
}

void SkillManager::add_skill(std::string name)
{
    if (name == "DoubleDamage")
        _skills.emplace_back(std::make_unique<DoubleDamage>());
    else if (name == "Scanner")
        _skills.emplace_back(std::make_unique<Scanner>());
    else if (name == "Bombing")
        _skills.emplace_back(std::make_unique<Bombing>());
    else
        throw std::invalid_argument("Invalid skill name for add!");
}

void SkillManager::clear()
{
    for (std::size_t i = 0; i < _skills.size(); ++i)
        _skills.pop_back();
}

void SkillManager::from_json(const json &j)
{
    this->clear();

    if (!j.is_null()) {
        for (std::size_t i = 0; i < j["skills"].size(); ++i)
            this->add_skill(j["skills"][i]);
    }
}

void SkillManager::show() const noexcept
{
    std::cout << "Current SkillManager abilities:\n";
    for (std::size_t i = 0; i < _skills.size(); ++i)
        std::cout << "-> " << _skills[i]->name() << "\n";
}
