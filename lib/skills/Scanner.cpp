#include "../../include/skills/Scanner.h"

#include <iostream>

#include "../../include/exceptions/OutOfBoundsException.h"

bool Scanner::apply(GameField& field, ShipManager& manager, std::size_t x, std::size_t y)
{
    if (x >= field.width() || y >= field.height())
        throw OutOfBoundsException("Can't scan space beyond the boundaries of the field!");

    bool have_ship = false;

    // right down corner of scan surface coordinates
    std::size_t x_e = (x + 1 >= field.width())  ? (field.width() - 1) : (x + 1);
    std::size_t y_e = (y + 1 >= field.height()) ? (field.height() - 1) : (y + 1);

    for (std::size_t x0 = 0; x0 <= x_e; ++x0) {
        for (std::size_t y0 = 0; y0 <= y_e; ++y0)
            have_ship = true;
    }

    if (have_ship) {
        std::cout << "There is ship in square from (" << x   << ";" << y   << ") to ("
                                                      << x_e << ";" << y_e << ")\n";
        return true;
    }
    
    std::cout << "There is no ship in square from (" << x   << ";" << y   << ") to ("
                                                     << x_e << ";" << y_e << ")\n";
    return false;
}

std::string Scanner::name() { return "Scanner"; }
