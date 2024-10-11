#ifndef SHIP_H
#define SHIP_H

#include <cstdint>
#include <vector>
#include <string>

class Ship {
public:
    enum class Size: uint8_t {Small = 1, Medium = 2, Large = 3, Huge = 4};
    enum class SegmentState: uint8_t {Intact = 2, Damaged = 1, Destroyed = 0};
    enum class Orientation: uint8_t {Vertical, Horizontal};

    // using coord = std::pair<std::size_t, std::size_t>;

    Ship(Size size);

    // operators
    Ship::SegmentState& operator [] (std::size_t segment_index);

    // getters    
    Size         size() const;
    SegmentState segment_state(std::size_t segment_index) const;
    Orientation  orientation() const;
    bool         placed() const;

    // setters
    void set_orientation(Orientation orientation);
    void set_placed();
    
    // main methods
    // void take_damage(std::size_t segment_index);
    bool destroyed() const;
    std::string str() const;
    
private:
    Size                      size_;
    std::vector<SegmentState> segments_;
    // std::vector<coord>        coords_; 
    Orientation               orientation_;
    bool                      placed_;
};

#endif
