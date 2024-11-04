#include "../include/Ship.h"

#include <stdexcept>
#include <iostream>
#include <string>

#define INVALID_INDEX "Index must be in range [0; ship_size - 1]!"

Ship::Ship(std::size_t size)
    : _size{std::move(size)},
      _vertical{false}
{
    // check size for correctness
    if (_size < static_cast<std::size_t>(Ship::Size::SMALL) ||
        _size > static_cast<std::size_t>(Ship::Size::HUGE))
        throw std::invalid_argument("Invalid ship size!");

    // fill segments vector
    for (std::size_t i {0}; i < _size; ++i)
        _segs.emplace_back(SegState::INTACT);

    // fill health of ship
    _health = _size * static_cast<std::size_t>(SegState::INTACT);
}

///////////////
// OPERATORS //
Ship::SegState &Ship::operator [] (std::size_t seg_index)
{
    if (seg_index >= _size)
        throw std::out_of_range(INVALID_INDEX);

    return _segs[seg_index];
}

/////////////
// GETTERS //
std::size_t                 Ship::size() const noexcept        { return _size; }
std::vector<Ship::SegState> Ship::segs() const noexcept        { return _segs; }
std::size_t                 Ship::health() const noexcept      { return _health; }
bool                        Ship::is_vertical() const noexcept { return _vertical; }

/////////////
// SETTERS //
void Ship::set_vertical() noexcept { _vertical = true; }

////////////////
// MAIN LOGIC //
void Ship::take_damage(std::size_t seg_index)
{
    if (seg_index >= static_cast<std::size_t>(_size))
        throw std::invalid_argument(INVALID_INDEX);

    if (_segs[seg_index] == SegState::INTACT)
        _segs[seg_index] = SegState::DAMAGED;
    else if (_segs[seg_index] == SegState::DAMAGED)
        _segs[seg_index] = SegState::DESTROYED;
}

std::string Ship::str() const
{
    std::string ship_view;
    for (std::size_t i {0}, size = _segs.size(); i < size; ++i) {
        ship_view += '[';
        ship_view += std::to_string(static_cast<int>(_segs[i]));
        ship_view += ']';
    }

    return ship_view;
}
