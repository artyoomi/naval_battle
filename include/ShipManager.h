#ifndef SHIP_MANAGER_H
#define SHIP_MANAGER_H

#include <iostream>
#include <vector>

#include "Ship.h"
#include "GameField.h"

/*
 * Responsibility:
 * Store information about all ships and their states.
 */

class ShipManager {
public:
    ShipManager(std::initializer_list<std::size_t> sizes);

    // getters
    std::size_t size() const noexcept;

    Ship &operator [] (std::size_t index) const;

    // main functions
    void show() const;

private:
    std::vector<ShipPtr> _ships;
};

#endif
