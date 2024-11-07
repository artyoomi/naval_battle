#ifndef SKILL_H
#define SKILL_H

#include "../GameField.h"
#include "../ShipManager.h"

class ISkill {
public:
    virtual void        apply(GameField& field, ShipManager& manager, std::size_t x = 0, std::size_t y = 0) = 0;
    virtual std::string name() = 0;
};

#endif
