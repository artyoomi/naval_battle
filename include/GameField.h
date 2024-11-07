#ifndef GAME_FIELD_H
#define GAME_FIELD_H

#include <cstdint>

#include <vector>
#include <stdexcept>
#include <memory>

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
        ShipPtr     _ship_ptr  = nullptr;
        std::size_t _seg_index = -1;
        bool        _is_hidden = true;
        CellState   _state     = CellState::UNKNOWN;

    public:
        Cell() = default;

        // operators
        bool operator == (const Cell &cell) const noexcept;

        // getters
        CellState state() const noexcept;
        Ship::SegState seg_state() const;
        bool           is_destroyed() const noexcept;

        bool is_ship() const noexcept;
        void set_ship_seg(ShipPtr const ship_ptr, const int seg_index) noexcept;
        void attack();
    };

public:
    static const std::size_t min_width;
    static const std::size_t max_width;

    static const std::size_t min_height;
    static const std::size_t max_height;
    
    // constructors
    GameField(std::size_t width, std::size_t height);
    GameField(const GameField& other);
    GameField(GameField&& other);
    
    // operators
    GameField& operator = (const GameField& other);
    GameField& operator = (GameField&& other);

    const std::vector<Cell>& operator [] (std::size_t x) const;

    // getters
    std::size_t width() const noexcept;
    std::size_t height() const noexcept;

    void set_double_damage();

    // main logic
    bool is_ship(std::size_t x, std::size_t y) const noexcept;
    void place_ship(const ShipPtr &ship_ptr, std::size_t x, std::size_t y, bool is_vertical);
    bool attack(std::size_t x, std::size_t y);

    void show() const;

private:
    std::size_t                    _width;
    std::size_t                    _height;
    std::vector<std::vector<Cell>> _field;
    bool                           _double_damage = false;
};

#endif
