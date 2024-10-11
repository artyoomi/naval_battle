#include "../include/Ship.h"

#include <stdexcept>
#include <iostream>
#include <string>

#define INVALID_INDEX "Index must be in range [0; ship_size - 1]!"

Ship::Ship(Size size)
    : size_{std::move(size)},
      segments_{static_cast<std::size_t>(size_), SegmentState::Intact},
      // coords_{static_cast<std::size_t>(size_), coord{0, 0}},
      orientation_{Orientation::Vertical},
      placed_{false}
{}
///////////////
// OPERATORS //
///////////////

Ship::SegmentState& Ship::operator [] (std::size_t segment_index)
{
    if (segment_index >= static_cast<std::size_t>(Ship::Size::Huge))
        throw std::out_of_range(INVALID_INDEX);

    return segments_[segment_index];
}

/////////////
// GETTERS //
/////////////

Ship::Size Ship::size() const
    {return size_;}

Ship::SegmentState Ship::segment_state(std::size_t segment_index) const
{
    if (segment_index < static_cast<std::size_t>(size_))
        return segments_[segment_index];
    else {
        throw std::invalid_argument(INVALID_INDEX);
    }
}

Ship::Orientation Ship::orientation() const
    {return orientation_;}

bool Ship::placed() const
    {return placed_;}

/////////////
// SETTERS //
/////////////

void Ship::set_orientation(Ship::Orientation orientation)
    {orientation_ = orientation;}
    
void Ship::set_placed()
    {placed_ = true;}

////////////////
// MAIN LOGIC //
////////////////

// void Ship::take_damage(std::size_t segment_index)
// {
//     if (segment_index < static_cast<std::size_t>(size_)) {
//         if (segments_[segment_index] == SegmentState::Intact)
//             segments_[segment_index] = SegmentState::Damaged;
//         else if (segments_[segment_index] == SegmentState::Damaged)
//             segments_[segment_index] = SegmentState::Destroyed;
//     } else {
//         throw std::invalid_argument(INVALID_INDEX);
//     }
// }

bool Ship::destroyed() const
{
    bool is_destroyed = true;
    for (std::size_t i {0}; i < static_cast<std::size_t>(size_); ++i) {
        if (segments_[i] != SegmentState::Destroyed)
            is_destroyed = false;
    }

    return is_destroyed;
}

std::string Ship::str() const
{
    std::string ship_view;
    for (std::size_t i {0}, size = segments_.size(); i < size; ++i) {
        ship_view += '[';
        ship_view += static_cast<std::size_t>(segments_[i]);
        ship_view += ']';
    }

    return ship_view;
}
