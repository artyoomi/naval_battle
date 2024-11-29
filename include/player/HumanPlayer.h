#ifndef HUMAN_PLAYER_H
#define HUMAN_PLAYER_H

#include "Player.h"

class HumanPlayer : public Player {
public:
    HumanPlayer();
    
    void make_turn() override;
    void place_ship() override;
    void attack() override;

private:
    bool _double_damage;
};

#endif
