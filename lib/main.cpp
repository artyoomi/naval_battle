#include <iostream>
#include <vector>

#include "../include/Ship.h"
#include "../include/ShipManager.h"
#include "../include/GameField.h"

int main()
{
    ShipManager manager({1, 4, 3, 2});
    manager.show();
    
    GameField field(5, 5);
    field.show();

//     manager.place_ship_to_field(field, 1, 4, 4, true);
//     manager.place_ship_to_field(field, 0, 2, 1, false);

    field.place_ship(&manager[0], 4, 4, true);

    manager.show();
    
    field.attack(4, 4);
    field.attack(4, 4);

    Ship ship(4);
    std::cout << ship.size() << ' ' << static_cast<std::size_t>(ship[3]) << '\n';
    
    field.show();

    manager.show();
    
    return 0;
}
