#include "../include/ShipManager.h"

#include <stdexcept>

#include "../include/Ship.h"
#include "../include/GameField.h"

#define E_INVALID_INDEX "Index out of range"
#define E_ALREADY_PLACED "The ship has already been placed on the field"
#define E_ALREADY_DESTROYED "Ship already destroyed"
#define E_WRONG_SIZE "Ship size must be in range [1; 4]"

ShipManager::ShipManager(std::initializer_list<std::size_t> sizes)
{
    for (const auto &size : sizes) {
        if (static_cast<Ship::Size>(size) < Ship::Size::SMALL ||
            static_cast<Ship::Size>(size) > Ship::Size::HUGE)
            throw std::logic_error(E_WRONG_SIZE);
        
        _inactive_ships.emplace_back(new Ship(size));
    }
}

ShipManager::~ShipManager()
{
    for (std::size_t i = 0, size = _inactive_ships.size(); i < size; ++i)
        delete _inactive_ships[i];
    for (std::size_t i = 0, size = _active_ships.size(); i < size; ++i)
        delete _active_ships[i];
}

/////////////
// GETTERS //
std::size_t ShipManager::size()          const noexcept { return _inactive_ships.size() + _active_ships.size(); }
std::size_t ShipManager::inactive_size() const noexcept { return _inactive_ships.size(); }
std::size_t ShipManager::active_size()   const noexcept { return _active_ships.size(); }

const Ship& ShipManager::get_inactive_ship(std::size_t index) const
{
    if (index >= inactive_size())
        throw std::out_of_range("Index is out of range!");
    return *_inactive_ships[index];
}

const Ship& ShipManager::get_active_ship(std::size_t index) const
{
    if (index >= active_size())
        throw std::out_of_range("Index is out of range!");
    return *_active_ships[index];
}

////////////////
// MAIN LOGIC //
void ShipManager::place_ship_to_field(GameField& field,
                                      std::size_t ship_index,
                                      std::size_t x, std::size_t y,
                                      bool is_vertical)
{
    if (inactive_size() == 0)
        throw std::logic_error("No available ships!");
    if (ship_index < 0 || ship_index >= inactive_size())
        throw std::invalid_argument("Invalid ship index!");

    // place ship on field
    field.place_ship(_inactive_ships[ship_index], x, y, is_vertical);

    std::move(_inactive_ships.begin() + ship_index,
              _inactive_ships.begin() + ship_index + 1,
              std::back_inserter(_active_ships));

    _inactive_ships.erase(_inactive_ships.begin() + ship_index);
}

void ShipManager::show() const
{
    std::cout << "Manager contain this active ships:\n";
    for (std::size_t i {0}, size = _active_ships.size(); i < size; ++i) {
        std::cout << i + 1 << ") ";
        std::cout << '[' << _active_ships[i]->str() << ']';
        std::cout << '\n';
    }
}
