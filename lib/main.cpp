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

    manager.place_ship(field, 2, 4, 4, Ship::Orientation::Horizontal);

    manager.show();
    
    field.attack(4, 4);
    field.attack(3, 4);
    field.attack(3, 3);
    field.attack(3, 0);
    
    field.show();
    
    return 0;
}
