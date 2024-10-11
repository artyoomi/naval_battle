#ifndef SHIP_MANAGER_H
#define SHIP_MANAGER_H

#include <iostream>
#include <vector>

#include "Ship.h"
#include "GameField.h"

class ShipManager {
public:
    ShipManager(std::initializer_list<std::size_t> sizes);

    // getters
    std::size_t size() const;

    // main functions
    void show() const;
    bool place_ship(GameField& field,
                    std::size_t ship_index,
                    std::size_t x, std::size_t y,
                    Ship::Orientation orientation);

private:
    std::vector<Ship> ships_;
};

#endif
