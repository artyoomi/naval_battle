#ifndef BOT_PLAYER_H
#define BOT_PLAYER_H

#include "Player.h"

class BotPlayer : public Player {
public:
    void make_turn() override;
    void place_ship() override;
    void attack() override;
};

#endif
