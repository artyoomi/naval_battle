#include "../include/GameField.h"

#include <stdexcept>

GameField::CellState GameField::Cell::state() const noexcept { return _state; }

Ship::SegState GameField::Cell::seg_state() const
{
    if (_ship_ptr == nullptr)
        throw std::logic_error("No ship in segment");

    return (*_ship_ptr)[_seg_index];
}

bool GameField::Cell::is_ship() const noexcept { return _state == CellState::SHIP; }

//sets ship segment to the cell, adding a pointer to ship
void GameField::Cell::set_ship_seg(Ship* const ship_ptr, const int seg_index) noexcept
{
    _ship_ptr = ship_ptr;
    _seg_index = seg_index;
}

//method to damage a ship by the segment if there is a ship
void GameField::Cell::attack()
{
    if (_ship_ptr == nullptr)
        _state = CellState::EMPTY;
    else {
        _ship_ptr->take_damage(_seg_index);
        _state = CellState::SHIP;
    }
}
