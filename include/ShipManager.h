#ifndef SHIP_MANAGER_H
#define SHIP_MANAGER_H

#include <iostream>
#include <vector>

#include "Ship.h"

/*
 * Responsibility:
 * Store information about all ships and their states.
 */

class ShipManager {
public:
    ShipManager(std::vector<std::size_t> sizes);
    ShipManager(const ShipManager &another);
    ShipManager(ShipManager &&another);
    ~ShipManager();

    // getters
    std::size_t size() const noexcept;

    void add_ship(std::size_t size);

    ShipManager& operator = (const ShipManager &another);
    ShipManager& operator = (ShipManager &&another);

    Ship &operator [] (std::size_t index) const;

    // main functions
    void show() const;

private:
    std::vector<ShipPtr> _ships;
};

#endif
