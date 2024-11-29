#ifndef BOMBING_H
#define BOMBING_H

#include "Skill.h"

class Bombing : public ISkill {
public:
    bool        apply(GameField& field, ShipManager& manager, std::size_t x = 0, std::size_t y = 0) override;
    std::string name() override;
};

#endif
