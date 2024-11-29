#include "../include/GameField.h"

#include "../include/Ship.h"
#include "../include/exceptions/OutOfBoundsException.h"
#include "../include/exceptions/ShipPlacementException.h"

#include <iostream>
#include <random>

const std::size_t GameField::min_width = 10;
const std::size_t GameField::max_width = 30;
const std::size_t GameField::min_height = 10;
const std::size_t GameField::max_height = 30;

//////////////////
// CONSTRUCTORS //
GameField::GameField(std::size_t width, std::size_t height)
    : _width{std::move(width)},
      _height{std::move(height)}
{
    if (_width  < min_width  || _width  > max_width ||
        _height < min_height || _height > max_height) {
        std::string err_msg = "Field sizes must be in bounds: ["
                            + std::to_string(min_width)  + ";" + std::to_string(max_width)
                            + "] for width and ["
                            + std::to_string(min_height) + ";" + std::to_string(max_height)
                            + "] for height!";
        throw std::invalid_argument(err_msg);
    }    

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

std::vector<GameField::Cell>& GameField::operator [] (std::size_t x)
{
    if (x >= this->width())
        throw std::out_of_range("Index of column is out of range!");

    return _field[x];
}

/////////////
// GETTERS //
std::size_t GameField::width()  const noexcept { return _width; }
std::size_t GameField::height() const noexcept { return _height; }

// void GameField::set_unhidden(std::size_t x, std::size_t y)
// {
//     if (x >= _width || y >= _height)
//         throw std::invalid_argument("x or y out of range in unidden func!");
//     _field[x][y].set_unhidden();
// }

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
    for (ssize_t x0 = left; x0 <= right; ++x0) {
        // from down to up
        for (ssize_t y0 = down; y0 <= up; ++y0) {
            
            if (x0 >= 0 && x0 < _width  &&
                y0 >= 0 && y0 < _height && _field[x0][y0].is_ship())
                throw ShipPlacementException("Intersection between ships occured!");
        }
    }

    if (is_vertical) { ship_ptr->set_vertical(); }

    std::size_t seg_index {0};
    for (std::size_t x_tail = x - x_offset; x_tail <= x; ++x_tail) {
        for (std::size_t y_tail = y - y_offset; y_tail <= y; ++y_tail)
            // ship_ptr->size() - seg_index++ to provide correct indexes of segments
            _field[x_tail][y_tail].set_ship_seg(ship_ptr, ship_ptr->size() - 1 - seg_index++);
    }
}

bool GameField::attack(std::size_t x, std::size_t y)
{
    if (x >= _width || y >= _height)
        throw OutOfBoundsException("Can't attack in out of bounds space!");
    
    _field[x][y].attack();

    _field[x][y].set_unhidden();

    return _field[x][y].is_destroyed();
}

// strange shit, need to fix later!
void GameField::random_placement(ShipManager &manager)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 200);
    
    for (std::size_t i = 0; i < manager.size(); ++i) {
        while (true) {
            try {
                this->place_ship(&manager[i], dist(gen), dist(gen), dist(gen) % 2);
                break;
            } catch (const std::exception &ex) {
                continue;
            }
        }
    }
}

json GameField::to_json() const
{
    json j;

    j["width"] = _width;
    j["height"] = _height;
    j["field"] = json();

    for (std::size_t x = 0; x < _width; ++x) {
        for (std::size_t y = 0; y < _height; ++y) {
            json cell_j;
            
            cell_j["seg_index"] = (_field[x][y].seg_index() == 0) ? (0) : (-1);
            cell_j["ship_size"] = (cell_j["seg_index"] == 0) ? (_field[x][y].ship_ptr()->size()) : (0);
            cell_j["is_vertical"] = (cell_j["seg_index"] == 0) ? (_field[x][y].ship_ptr()->vertical()) : (false);
            cell_j["is_hidden"] = _field[x][y].hidden();
            cell_j["state"] = static_cast<int>(_field[x][y].state());
            cell_j["x"] = x;
            cell_j["y"] = y;

            if (_field[x][y].ship_ptr() != nullptr)
                cell_j["hp"] = _field[x][y].ship_ptr()->segs();
            
            j["field"].push_back(cell_j);
        }
    }

    return j;
}

void GameField::from_json(const json &j, ShipManager &manager)
{
    GameField field(j["width"], j["height"]);

    // std::cout << j["width"] << " " << j["height"] << std::endl;

    std::vector<std::size_t> ship_sizes;

    for (std::size_t x = 0; x < field.width(); ++x) {
        for (std::size_t y = 0; y < field.height(); ++y) {
            std::size_t plain_coord = field.width() * x + y;
        
            // std::cout << "Here working already 1!\n";
            if (j["field"][plain_coord]["is_hidden"] == false)
                field[x][y].set_unhidden();
            
            // std::cout << "Here working already 2!\n";
            field[x][y].set_state(static_cast<GameField::CellState>(j["field"][plain_coord]["state"]));

            // std::cout << "Here working already 3!\n";
            if (j["field"][plain_coord]["seg_index"] == 0) {
                manager.add_ship(j["field"][plain_coord]["ship_size"]);
                // std::cout << "Current coords " << x << " " << y << std::endl;
                // std::cout << "Current json coord " << j["field"][field.width() * x + y] << std::endl;
                field.place_ship(&manager[manager.size() - 1], x, y, j["field"][plain_coord]["is_vertical"]);
            }

            // std::cout << "Current coords " << x << " " << y << std::endl;

            if (j["field"][plain_coord].find("hp") != j["field"][plain_coord].end() &&
                !j["field"][plain_coord]["hp"].is_null()) {
                // std::cout << "Current size is " << j["field"][plain_coord]["hp"].size() << std::endl;
                for (std::size_t i = 0; i < j["field"][plain_coord]["hp"].size(); ++i) {
                    // std::cout << j["field"][plain_coord]["hp"][i] << std::endl;
                    std::size_t damage = static_cast<std::size_t>(Ship::SegState::INTACT) - j["field"][plain_coord]["hp"][i].get<std::size_t>();
                    // std::cout << "Damage is equal to " << damage << std::endl;
                    for (std::size_t j = 0; j < damage; ++j)
                        field.attack(x, y);
                }
            }
        }
    }
    
    *this = field;
}

void GameField::show(bool covered) const
{
    auto str = [covered](Cell cell)
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
            if (covered || !cell.is_ship()) {
                str += '~';
                break;
            }
        case CellState::SHIP:
            if (cell.seg_state() == Ship::SegState::DAMAGED)
                str += '*';
            else if (cell.seg_state() == Ship::SegState::DESTROYED)
                str += 'X';
            else
                str += 'o';
            break;
        case CellState::EMPTY:
            str += ' ';
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
