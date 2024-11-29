#include "../include/GameField.h"

#include <stdexcept>

bool GameField::Cell::operator == (const Cell &cell) const noexcept
{
    return _ship_ptr   == cell._ship_ptr &&
           _seg_index  == cell._seg_index &&
           _is_hidden  == cell._is_hidden &&
           _state      == cell._state;
}

ShipPtr GameField::Cell::ship_ptr() const noexcept { return _ship_ptr; }

int GameField::Cell::seg_index() const noexcept { return _seg_index; }

GameField::CellState GameField::Cell::state() const noexcept { return _state; }

Ship::SegState GameField::Cell::seg_state() const
{
    if (_ship_ptr == nullptr)
        throw std::logic_error("No ship in segment");

    return (*_ship_ptr)[_seg_index];
}

void GameField::Cell::set_state(GameField::CellState state) { _state = state; }
void GameField::Cell::set_unhidden() { _is_hidden = false; }
bool GameField::Cell::hidden() const noexcept { return _is_hidden; }

bool GameField::Cell::is_destroyed() const noexcept
{
    if (_ship_ptr == nullptr || _ship_ptr->health())
        return false;
    return true;
}

bool GameField::Cell::is_ship() const noexcept { return _ship_ptr != nullptr; }

//sets ship segment to the cell, adding a pointer to ship
void GameField::Cell::set_ship_seg(ShipPtr const ship_ptr, const int seg_index) noexcept
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
