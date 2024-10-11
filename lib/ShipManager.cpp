#include "../include/ShipManager.h"

#include <stdexcept>
#include <iomanip>

#define E_INVALID_INDEX "Index out of range"
#define E_ALREADY_PLACED "The ship has already been placed on the field"
#define E_ALREADY_DESTROYED "Ship already destroyed"
#define E_WRONG_SIZE "Ship size must be in range [1; 4]"

ShipManager::ShipManager(std::initializer_list<std::size_t> sizes)
{
    for (const auto& size : sizes) {
        if (static_cast<Ship::Size>(size) < Ship::Size::Small ||
            static_cast<Ship::Size>(size) > Ship::Size::Huge)
            throw std::logic_error(E_WRONG_SIZE);
        
        ships_.emplace_back(Ship(static_cast<Ship::Size>(size)));
    }
}

/////////////
// GETTERS //
/////////////

std::size_t ShipManager::size() const
    {return ships_.size();}

////////////////
// MAIN LOGIC //
////////////////

void ShipManager::show() const
{
    // // size of 8 brackets, 4 numbers and 1 space
    // const std::size_t max_len = 13;
    
    // for (std::size_t i {0}, size = ships_.size(); i < size; ++i) {
    //     std::cout << std::setw(max_len) << ships_[i].str() << ' ';
    //     std::cout << ((ships_[i].placed()) ? ("placed") : ("not placed")) << std::endl;
    // }
    std::cout << "Manager contain this ships:\n";
    for (std::size_t i {0}, size = ships_.size(); i < size; ++i) {
        std::cout << i + 1 << ") ";
        for (std::size_t j {0}, size2 = static_cast<std::size_t>(ships_[i].size()); j < size2; ++j) {
            std::cout << '[' << static_cast<std::size_t>(ships_[i].segment_state(j)) << ']';
        }
        std::cout << '\n';
    }
}

bool ShipManager::place_ship(GameField& field,
                             std::size_t ship_index,
                             std::size_t x, std::size_t y,
                             Ship::Orientation orientation)
{
    if (ship_index < static_cast<std::size_t>(ships_[ship_index].size())) {
        if (ships_[ship_index].placed())
            throw std::logic_error(E_ALREADY_PLACED);
        // else if (ships_[ship_index].destroyed())
        //     throw std::logic_error(E_ALREADY_DESTROYED);

        try {
            if (field.place_ship(ships_[ship_index], x, y, orientation)) {
                ships_[ship_index].set_placed();
                return true;
            } else
                return false;
        } catch (const std::out_of_range& ex) {
            std::cerr << "Placement failure: " << ex.what() << std::endl;
        } catch (const std::runtime_error& ex) {
            std::cerr << "Placement failure: " << ex.what() << std::endl;
        }

        return false;
    }

    throw std::out_of_range(E_INVALID_INDEX);
}
