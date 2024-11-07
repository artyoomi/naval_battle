#include "../include/GameField.h"

#include "../include/Ship.h"
#include "../include/exceptions/OutOfBoundsException.h"
#include "../include/exceptions/ShipPlacementException.h"

#include <iostream>

const std::size_t GameField::min_width = 5;
const std::size_t GameField::max_width = 30;
const std::size_t GameField::min_height = 5;
const std::size_t GameField::max_height = 30;

//////////////////
// CONSTRUCTORS //
GameField::GameField(std::size_t width, std::size_t height)
    : _width{std::move(width)},
      _height{std::move(height)}
{
    bool changed = false;

    // check width
    if (width < min_width) {
        changed = true;
        _width = min_width;
    } else if (width > max_width) {
        changed = true;
        _width = max_width;
    }
    
    // check height
    if (height < min_height) {
        changed = true;
        _height = min_height;
    } else if (height > max_height) {
        changed = true;
        _height = max_height;
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
    : _width{min_width},
      _height{min_height}
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

const std::vector<GameField::Cell>& GameField::operator [] (std::size_t x) const
{
    if (x >= this->width())
        throw std::out_of_range("Index of column is out of range!");

    return _field[x];
}

/////////////
// GETTERS //
std::size_t GameField::width()  const noexcept { return _width; }
std::size_t GameField::height() const noexcept { return _height; }

void GameField::set_double_damage() { _double_damage = true; }

////////////////
// MAIN LOGIC //
bool GameField::is_ship(std::size_t x, std::size_t y) const noexcept { return _field[x][y].is_ship(); }

void GameField::place_ship(const ShipPtr &ship_ptr, std::size_t x, std::size_t y, bool is_vertical)
{
    // check ship pointer
    if (ship_ptr == nullptr)
        throw std::invalid_argument("Ship pointer is nullptr!");

    std::size_t x_offset = 0;
    std::size_t y_offset = 0;
    
    // set offsets
    if (!is_vertical)
        x_offset = ship_ptr->size() - 1;
    else
        y_offset = ship_ptr->size() - 1;

    // check coords for correctness
    if (x >= _width || y >= _height ||
        x < x_offset || y < y_offset)
        throw OutOfBoundsException("Invalid coordinates to place!");

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
                throw ShipPlacementException("Intersection between ships occured!");
        }
    }

    if (is_vertical) { ship_ptr->set_vertical(); }

    std::size_t seg_index {0};
    for (std::size_t x_tail = x - x_offset; x_tail <= x; ++x_tail) {
        for (std::size_t y_tail = y - y_offset; y_tail <= y; ++y_tail)
            _field[x_tail][y_tail].set_ship_seg(ship_ptr, seg_index++);
    }
}

bool GameField::attack(std::size_t x, std::size_t y)
{
    if (x >= _width || y >= _height)
        throw OutOfBoundsException("Can't attack in out of bounds space!");
    
    _field[x][y].attack();
    if (_double_damage)
        _field[x][y].attack();

    return _field[x][y].is_destroyed();
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
