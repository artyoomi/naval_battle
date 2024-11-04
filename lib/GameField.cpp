#include "../include/GameField.h"

#include "../include/Ship.h"

#include <iostream>

//////////////////
// CONSTRUCTORS //
GameField::GameField(std::size_t width, std::size_t height)
    : _width{std::move(width)},
      _height{std::move(height)}
{
    bool changed = false;

    // check width
    if (width < min_field_width) {
        changed = true;
        _width = min_field_width;
    } else if (width > max_field_width) {
        changed = true;
        _width = max_field_width;
    }
    
    // check height
    if (height < min_field_height) {
        changed = true;
        _height = min_field_height;
    } else if (height > max_field_height) {
        changed = true;
        _height = min_field_height;
    }

    // print warning message
    if (changed)
        std::cerr << "The input sizes exceed the allowed ones, the input data will be adjusted\n";

    // fill field
    _field.resize(_width);
    for (std::size_t x = 0; x < _width; ++x) {
        _field[x].resize(_height);
        for (std::size_t y = 0; y < _height; ++y)
            _field[x][y] = Cell();
    }
}

GameField::GameField(const GameField& other)
    : _width{other._width},
      _height{other._height},
      _field{other._field}
{}

GameField::GameField(GameField&& other)
    : _width{min_field_width},
      _height{min_field_height}
{
    std::swap(_width, other._width);
    std::swap(_height, other._height);
    std::swap(_field, other._field);
}

///////////////
// OPERATORS //
GameField& GameField::operator = (const GameField& other)
{
    if (&other != this) {
        GameField tmp{other};
        std::swap(_width, tmp._width);
        std::swap(_height, tmp._height);
        std::swap(_field, tmp._field);
    }
    
    return *this;
}

GameField& GameField::operator = (GameField&& other)
{
    if (&other != this) {
        std::swap(_width, other._width);
        std::swap(_height, other._height);
        std::swap(_field, other._field);
    }

    return *this;
}

/////////////
// GETTERS //
std::size_t GameField::width() const noexcept  { return _width; }
std::size_t GameField::height() const noexcept { return _height; }

////////////////
// MAIN LOGIC //
bool GameField::is_ship(std::size_t x, std::size_t y) const noexcept { return _field[x][y].is_ship(); }

void GameField::place_ship(Ship *ship, std::size_t x, std::size_t y, bool is_vertical)
{
    // check ship pointer
    if (ship == nullptr)
        throw std::invalid_argument("Ship pointer is nullptr!");

    std::size_t x_offset = 0;
    std::size_t y_offset = 0;
    
    // set offsets
    if (!is_vertical)
        x_offset = ship->size() - 1;
    else
        y_offset = ship->size() - 1;

    // check coords for correctness
    if (x >= _width || y >= _height ||
        x < x_offset || y < y_offset)
        throw std::out_of_range("Invalid ship coordinates!");

    /*
     * horizontal: x_tail <= x_head (x)
     * vertical:   y_tail <= y_head (y)
    */

    /* Check cells about new ship:
     * check from left up corner to right down
     * example:
     * ->[] ~ ~ ~ ~
     *    ~ * * * ~
     *    ~ ~ ~ ~ []<- right down corner
     *
     * count quantity of ship elements
     * if quantity != 0 => can't place ship, due to ships intersection
     * else => all right
    */
    ssize_t left  = static_cast<ssize_t>(x) - x_offset - 1;
    ssize_t right = static_cast<ssize_t>(x) + 1;
    ssize_t down  = static_cast<ssize_t>(y) - y_offset - 1;
    ssize_t up    = static_cast<ssize_t>(y) + 1;

    // from left to right
    for (ssize_t x = left; x < right; ++x) {
        // from down to up
        for (ssize_t y = down; y < up; ++y) {
            
            if (x >= 0 && x < _width  &&
                y >= 0 && y < _height && _field[x][y].is_ship())
                throw std::logic_error("Intersection between ships occured!");
        }
    }

    if (is_vertical) { ship->set_vertical(); }

    std::size_t seg_index {0};
    for (std::size_t x_tail = x - x_offset; x_tail <= x; ++x_tail) {
        for (std::size_t y_tail = y - y_offset; y_tail <= y; ++y_tail)
            _field[x_tail][y_tail].set_ship_seg(ship, seg_index++);
    }
}

void GameField::attack(std::size_t x, std::size_t y, std::size_t damage)
{
    if (x >= _width || y >= _height)
        throw std::out_of_range(E_INVALID_COORDS);
    
    damage = (damage < 2) ? (damage) : (2);

    for (std::size_t i = 0; i < damage; ++i)
        _field[x][y].attack();
}

void GameField::show() const
{
    auto str = [](Cell cell)
    {
        /*
         * [~] - unknown cell
         * [ ] - empty cell
         * [*] - damaged ship segment
         * [X] - destroyed ship segment
         */
        std::string str {"["};

        switch (cell.state()) {
        case CellState::UNKNOWN:
            str += '~';
            break;
        case CellState::EMPTY:
            str += ' ';
            break;
        case CellState::SHIP:
            if (cell.seg_state() == Ship::SegState::DAMAGED)
                str += '*';
            else if (cell.seg_state() == Ship::SegState::DESTROYED)
                str += 'X';
            else
                str += 'o';
            break;
    }

    str += ']';

    return str;
    };

    for (size_t y {0}; y < _height; ++y) {
        for (size_t x {0}; x < _width; ++x)
            std::cout << str(_field[x][y]);

        std::cout << '\n';
    }

    std::cout << '\n';
}
