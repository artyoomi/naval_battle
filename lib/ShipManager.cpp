#include "../include/ShipManager.h"

#include <stdexcept>

#include "../include/Ship.h"
#include "../include/GameField.h"

#define E_INVALID_INDEX "Index out of range"
#define E_ALREADY_PLACED "The ship has already been placed on the field"
#define E_ALREADY_DESTROYED "Ship already destroyed"
#define E_WRONG_SIZE "Ship size must be in range [1; 4]"

ShipManager::ShipManager(std::vector<std::size_t> sizes)
{
    for (const auto &size : sizes) {
        if (static_cast<Ship::Size>(size) < Ship::Size::SMALL ||
            static_cast<Ship::Size>(size) > Ship::Size::HUGE)
            throw std::logic_error(E_WRONG_SIZE);
        
        _ships.emplace_back(new Ship(size));
    }
}

ShipManager::ShipManager(const ShipManager &another)
    : _ships{another._ships}
{
}

ShipManager::ShipManager(ShipManager &&another)
    : _ships{}
{
    std::swap(_ships, another._ships);
}

ShipManager& ShipManager::operator = (const ShipManager &another)
{
    if (&another != this) {
        ShipManager tmp{another};
        std::swap(_ships, tmp._ships);
    }

    return *this;
}

ShipManager& ShipManager::operator = (ShipManager &&another)
{
    if (&another != this)
        std::swap(_ships, another._ships);

    return *this;
}

ShipManager::~ShipManager()
{
    for (std::size_t i = 0; i < _ships.size(); ++i)
        delete _ships[i];
}

/////////////
// GETTERS //
std::size_t ShipManager::size() const noexcept { return _ships.size(); }

void ShipManager::add_ship(std::size_t size)
{
    _ships.emplace_back(new Ship(size));
}

Ship& ShipManager::operator [] (std::size_t index) const
{
    if (index >= _ships.size())
        throw std::out_of_range("Index is out of range!");
    return *_ships[index];
}

void ShipManager::show() const
{
    std::cout << "Manager contain this ships:\n";
    for (std::size_t i {0}, size = _ships.size(); i < size; ++i) {
        std::cout << i + 1 << ") ";
        std::cout << '[' << _ships[i]->str() << ']';
        std::cout << '\n';
    }
}
