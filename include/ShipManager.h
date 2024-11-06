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
    ~ShipManager();

    // getters
    std::size_t size() const noexcept;
    std::size_t inactive_size() const noexcept;
    std::size_t active_size()   const noexcept;

    const Ship& inactive_ship(std::size_t index) const;
    const Ship& active_ship(std::size_t index) const;

    void place_ship_to_field(GameField &field, std::size_t ship_index,
                             std::size_t x, std::size_t y, bool is_vertical);

    // main functions
    void show() const;

private:
    std::vector<Ship*> _inactive_ships;
    std::vector<Ship*> _active_ships;
};

#endif
