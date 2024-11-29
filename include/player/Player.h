#ifndef PLAYER_H
#define PLAYER_H

#include "../GameField.h"
#include "../ShipManager.h"
#include "../SkillManager.h"

class Player {
public:
    virtual void make_turn() = 0;
    virtual void place_ship() = 0;
    virtual void attack() = 0;

    void set_field(GameField field) { _field = std::move(field); }
    void set_ship_manager(ShipManager manager) { _ship_manager = std::move(manager); }

    GameField& field() { return _field; }
    ShipManager& ship_manager() { return _ship_manager; }

protected:
    GameField    _field;
    ShipManager  _ship_manager;
};

#endif
