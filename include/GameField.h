#ifndef GAME_FIELD_H
#define GAME_FIELD_H

#include <cstdint>

#include <vector>

#include "Ship.h"

#define E_INVALID_COORDS "Coordinates are out of range"
#define E_PLCMNT_ERROR "It is impossible to place the ship according to these coordinates"

/*
 * Responsibility:
 * 1. Store information about field cells;
 * 2. Provide interface to:
 *    - Attack cell;
 *    - Place ship on field.
 */

class GameField {
public:
    enum class CellState : uint8_t {UNKNOWN, EMPTY, SHIP};
private:
    class Cell {
    private:
        Ship *_ship_ptr  = nullptr;
        ssize_t _seg_index   = -1;
        CellState _state = CellState::UNKNOWN;

    public:
        Cell() = default;

        CellState state() const noexcept;

        Ship::SegState seg_state() const;

        //returns true if there is a ship in cell
        bool is_ship() const noexcept;

        //sets ship segment to the cell, adding a pointer to ship
        void set_ship_seg(Ship *const ship_ptr, const int seg_index) noexcept;

        //method to damage a ship by the segment if there is a ship
        void attack();
    };

public:
    const std::size_t min_field_width = 5;
    const std::size_t max_field_width = 30;

    const std::size_t min_field_height = 5;
    const std::size_t max_field_height = 30;
    
    // constructors
    GameField(std::size_t width, std::size_t height);
    GameField(const GameField& other);
    GameField(GameField&& other);
    
    // operators
    GameField& operator = (const GameField& other);
    GameField& operator = (GameField&& other);

    // getters
    std::size_t width() const noexcept;
    std::size_t height() const noexcept;

    // main logic
    bool is_ship(std::size_t x, std::size_t y) const noexcept;
    void place_ship(Ship *ship, std::size_t x, std::size_t y, bool is_vertical);
    void attack(std::size_t x, std::size_t y, std::size_t damage = 1);
    void show() const;

private:
    std::size_t                    _width;
    std::size_t                    _height;
    std::vector<std::vector<Cell>> _field;
};

#endif
