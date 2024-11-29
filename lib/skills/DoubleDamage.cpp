#include "../../include/skills/DoubleDamage.h"

bool DoubleDamage::apply(GameField& field, ShipManager& manager, std::size_t x, std::size_t y)
{
    return true;
}

std::string DoubleDamage::name() { return "DoubleDamage"; }
