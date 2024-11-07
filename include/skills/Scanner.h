#ifndef SCANNER_H
#define SCANNER_H

#include "Skill.h"

class Scanner : public ISkill {
public:
    void        apply(GameField& field, ShipManager& manager, std::size_t x = 0, std::size_t y = 0) override;
    std::string name() override;
};

#endif
