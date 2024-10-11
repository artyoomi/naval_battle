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
    
    field.attack(5, 5);
    field.attack(4, 5);
    field.attack(3, 5);
    
    field.show();
    
    return 0;
}
