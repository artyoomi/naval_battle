#include <iostream>
#include <vector>

#include "../include/Ship.h"
#include "../include/ShipManager.h"
#include "../include/GameField.h"
#include "../include/SkillManager.h"

int main()
{
    ShipManager  sh_manager({1, 2, 3, 4, 4, 3, 2, 1});
    GameField    field(10, 10);
    SkillManager sk_manager;

    field.place_ship(&sh_manager[0], 0, 0, false);

    sh_manager.show();
    field.show();
    sk_manager.show();
    std::cout << std::endl;

    if (field.attack(0, 0))
        sk_manager.add_random_skill();

    sh_manager.show();
    field.show();
    sk_manager.show();

    if (field.attack(0, 0))
        sk_manager.add_random_skill();

    sh_manager.show();
    field.show();
    sk_manager.show();
    
    return 0;
}
