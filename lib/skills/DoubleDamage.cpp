#include "../../include/skills/DoubleDamage.h"

void DoubleDamage::apply(GameField& field, ShipManager& manager, std::size_t x, std::size_t y)
{
    field.set_double_damage();
}

std::string DoubleDamage::name() { return "DoubleDamage"; }
