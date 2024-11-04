#ifndef SHIP_H
#define SHIP_H

#include <cstdint>

#include <vector>
#include <string>

/*
 * Responsibility:
 * Store information about individual ship.
 */

class Ship {
public:
    enum class Size: uint8_t {SMALL = 1, MEDIUM = 2, LARGE = 3, HUGE = 4};
    enum class SegState: uint8_t {INTACT = 2, DAMAGED = 1, DESTROYED = 0};

    // constructors
    Ship(std::size_t size);

    // operators
    Ship::SegState& operator [] (std::size_t seg_index);

    // getters
    std::size_t           size() const noexcept;
    std::vector<SegState> segs() const noexcept;
    std::size_t           health() const noexcept;
    bool                  is_vertical() const noexcept;

    // setters
    void set_vertical() noexcept;
    
    // main methods
    void take_damage(std::size_t seg_index);

    std::string str() const;
    
private:
    std::size_t           _size;
    std::vector<SegState> _segs;
    std::size_t           _health;
    bool                  _vertical;
};

#endif
