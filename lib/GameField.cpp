#include "../include/GameField.h"

#include "../include/Ship.h"

#include <iostream>

//////////////////
// CONSTRUCTORS //
//////////////////

GameField::GameField(std::size_t width, std::size_t height)
    : width_{std::move(width)},
      height_{std::move(height)},
      field_{width, std::vector<CellInfo>{height}}
{
    bool changed {false};
    if (width < MIN_FIELD_WIDTH) {
        changed = true;
        width_ = MIN_FIELD_WIDTH;
    } else if (width > MAX_FIELD_WIDTH) {
        changed = true;
        width_ = MAX_FIELD_WIDTH;
    }
    
    if (height < MIN_FIELD_HEIGHT) {
        changed = true;
        height_ = MIN_FIELD_HEIGHT;
    } else if (height > MAX_FIELD_HEIGHT) {
        changed = true;
        height_ = MIN_FIELD_HEIGHT;
    }

    if (changed)
        std::cerr << "The input sizes exceed the allowed ones, the input data will be adjusted\n"; 
}

GameField::GameField(const GameField& other)
    : width_{other.width_},
      height_{other.height_},
      field_{other.field_}
{}

GameField::GameField(GameField&& other)
    : width_{0},
      height_{0}
{
    std::swap(width_, other.width_);
    std::swap(height_, other.height_);
    std::swap(field_, other.field_);
}

GameField::CellInfo::CellInfo()
    : state_{CellState::Unknown},
      system_state_{CellState::Empty},
      segment_{nullptr}
{}

GameField::CellInfo::CellInfo(const CellInfo& other)
    : state_{other.state_},
      system_state_{other.system_state_},
      segment_{other.segment_}
{}

///////////////
// OPERATORS //
///////////////

GameField& GameField::operator = (const GameField& other)
{
    if (&other != this) {
        GameField tmp{other};
        std::swap(width_, tmp.width_);
        std::swap(height_, tmp.height_);
        std::swap(field_, tmp.field_);
    }
    
    return *this;
}

GameField& GameField::operator = (GameField&& other)
{
    if (&other != this) {
        std::swap(width_, other.width_);
        std::swap(height_, other.height_);
        std::swap(field_, other.field_);
    }

    return *this;
}

GameField::CellInfo& GameField::CellInfo::operator = (const CellInfo& other)
{
    if (&other != this) {
        CellInfo tmp{other};
        std::swap(state_, tmp.state_);
        std::swap(system_state_, tmp.system_state_);
        std::swap(segment_, tmp.segment_);
    }

    return *this;
}

/////////////
// GETTERS //
/////////////

std::size_t GameField::width() const
    {return width_;}
std::size_t GameField::height() const
    {return height_;}

GameField::CellState GameField::CellInfo::state() const
    {return state_;}

GameField::CellState GameField::CellInfo::system_state() const
    {return system_state_;}

/////////////
// SETTERS //
/////////////

void GameField::CellInfo::set_state(GameField::CellState new_state)
    {state_ = new_state;}

void GameField::CellInfo::set_system_state(CellState system_state)
    {}
void GameField::CellInfo::set_segment_ptr(Ship::SegmentState& seg_state)
    {segment_ = std::make_shared<Ship::SegmentState>(seg_state);}

////////////////
// MAIN LOGIC //
////////////////

bool GameField::place_ship(Ship& ship,
                           std::size_t x, std::size_t y,
                           Ship::Orientation orientation)
{
    if (x >= width_ || y >= height_)
        throw std::out_of_range(E_INVALID_COORDS);

    std::size_t x_offset {0};
    std::size_t y_offset {0};
    
    if (orientation == Ship::Orientation::Horizontal)
        x_offset = static_cast<std::size_t>(ship.size()) - 1;
    else
        y_offset = static_cast<std::size_t>(ship.size()) - 1;

    /*
    for a while:
    horizontal: x_tail <= x_head (x)
    vertical:   y_tail <= y_head (y)
    */

    // cant place at this position
    if (x < x_offset || y < y_offset)
        throw std::runtime_error(E_PLCMNT_ERROR);

    /*
    check from left up corner to right down
    example:
      ->[] ~ ~ ~ ~
        ~ * * * ~
        ~ ~ ~ ~ []<- right down corner
        
    count quantity of ship elements
    if quantity != ship.size() => wrong placement
    else => all right
    */

    std::size_t ship_segments_count {0};

    // check area around ship
    // from left to right
    for (ssize_t left {static_cast<ssize_t>(x - x_offset) - 1}, right {static_cast<ssize_t>(x) + 1};
         left <= right; ++left) {
        
        // from down to up
        for (ssize_t down {static_cast<ssize_t>(y - y_offset) - 1}, up {static_cast<ssize_t>(y) + 1};
             down <= up; ++down) {
            
            if (left >= 0 && down >= 0 &&
                left < width_ && down < height_ &&
                field_[left][down].system_state() == CellState::Ship)
                ++ship_segments_count;
        }
    }

    if (ship_segments_count > static_cast<std::size_t>(ship.size()))
        return false;

    ship.set_orientation(orientation);

    std::size_t segment_index {0};
    for (std::size_t x_tail {x - x_offset}; x_tail <= x; ++x_tail) {
        for (std::size_t y_tail {y - y_offset}; y_tail <= y; ++y_tail) {
            field_[x_tail][y_tail].set_system_state(CellState::Ship);
            field_[x_tail][y_tail].set_segment_ptr(ship[segment_index++]);
        }
    }
    
    return true;
}

void GameField::attack(std::size_t x, std::size_t y)
{
    if (x >= width_ || y >= height_)
        throw std::out_of_range(E_INVALID_COORDS);
    
    field_[x][y].attack();

    if (field_[x][y].is_ship())
        field_[x][y].set_state(CellState::Ship);
    else
        field_[x][y].set_state(CellState::Empty);
}

void GameField::show() const
{
    for (size_t y {0}; y < height_; ++y) {
        for (size_t x {0}; x < width_; ++x)
            std::cout << field_[x][y].str();

        std::cout << '\n';
    }

    std::cout << '\n';
}

std::string GameField::CellInfo::str() const
{
    /*
    [~] - unknown cell
    [ ] - empty cell
    [*] - damaged ship segment
    [X] - destroyed ship segment
    */
    std::string str {"["};

    switch (system_state_) {
    case GameField::CellState::Unknown:
        str += '~';
        break;     
    case GameField::CellState::Empty:
        str += ' ';
        break;   
    case GameField::CellState::Ship:
        if (*segment_ == Ship::SegmentState::Damaged)
            str += '*';
        else if (*segment_ == Ship::SegmentState::Destroyed)
            str += 'X';
        break;
    }

    str += ']';
    
    return str;
}

bool GameField::CellInfo::is_ship() const
    {return system_state_ == CellState::Ship;}

void GameField::CellInfo::attack()
{
    if (segment_ == nullptr)
        return;
    else if (*segment_ == Ship::SegmentState::Intact)
        *segment_ = Ship::SegmentState::Damaged;
    else if (*segment_ == Ship::SegmentState::Damaged)
        *segment_ = Ship::SegmentState::Destroyed;
    else
        throw std::logic_error("Segment already destroyed");
        
}
