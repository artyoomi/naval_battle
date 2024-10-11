#ifndef GAME_FIELD_H
#define GAME_FIELD_H

#include <cstdint>
#include <vector>
#include <memory>

#include "Ship.h"

#define MIN_FIELD_WIDTH 5
#define MAX_FIELD_WIDTH 30

#define MIN_FIELD_HEIGHT 5
#define MAX_FIELD_HEIGHT 30

#define E_INVALID_COORDS "Coordinates are out of range"
#define E_PLCMNT_ERROR "It is impossible to place the ship according to these coordinates"

class GameField {
public:
    enum class CellState: uint8_t {Unknown, Empty, Ship};
    
    // constructors
    GameField(std::size_t width = 0, std::size_t height = 0);
    GameField(const GameField& other);
    GameField(GameField&& other);
    
    // operators
    GameField& operator = (const GameField& other);
    GameField& operator = (GameField&& other);

    // getters
    std::size_t width() const;
    std::size_t height() const;

    // main logic
    bool place_ship(Ship& ship,
                    std::size_t x, std::size_t y,
                    Ship::Orientation orient);
    void attack(std::size_t x, std::size_t y);
    void show() const;

private:
    class CellInfo {
    public:
        // constructors
        CellInfo();
        CellInfo(const CellInfo& other);

        // operators
        CellInfo& operator = (const CellInfo& other);

        // getters
        CellState state() const;
        CellState system_state() const;

        // setters
        void set_state(CellState state);
        void set_system_state(CellState system_state);
        void set_segment_ptr(Ship::SegmentState& seg_state);

        // main methods
        std::string str() const;
        bool is_ship() const;
        void attack();

    private:
        CellState state_;
        CellState system_state_;
        std::shared_ptr<Ship::SegmentState> segment_;
    };

    std::size_t width_;
    std::size_t height_;
    std::vector<std::vector<CellInfo>> field_;
};

#endif
