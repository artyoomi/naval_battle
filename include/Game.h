#ifndef GAME_H
#define GAME_H

#include "GameField.h"
#include "ShipManager.h"
#include "SkillManager.h"
#include "save_load/GameState.h"

class Game {
public:
    Game();
    
    std::vector<std::size_t> input_coords();
    void start_game();
    void make_turn();

private:
    // player data
    GameField    _p_field;
    ShipManager  _p_ship_manager;
    SkillManager _p_skill_manager;
    bool         _double_damage = false;
    GameState    _state;

    // bot data
    GameField    _b_field;
    ShipManager  _b_ship_manager;

    bool _running = false;
    bool _bot_attack = false;
};

#endif
