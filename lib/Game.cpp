#include "../include/Game.h"

#include <limits>
#include <random>
#include "../include/exceptions/NoSkillsException.h"

Game::Game()
    // just a mock, fix later
    : _p_field(GameField::min_width, GameField::min_height),
      _p_ship_manager({}),
      _b_field(GameField::min_width, GameField::min_height),
      _b_ship_manager({}),
      _p_skill_manager(_p_field, _p_ship_manager),
      _state{"save.json"}
{}

std::vector<std::size_t> Game::input_coords()
{
    std::string x_str, y_str;
    std::size_t x, y;

    // input x and y
    while (true) {
        std::cout << "Input x: ";
        getline(std::cin, x_str);
        try {
            x = stoi(x_str);
        } catch (std::exception &ex) {
            std::cerr << "Wrong x, try again!\n";
            continue;
        }
        
        if (x < 0 || x >= _p_field.width())
            std::cerr << "Wrong x, try again!\n";
        else
            break;
    }
    
    while (true) {
        std::cout << "Input y: ";
        getline(std::cin, y_str);
        try {
            y = stoi(y_str);
        } catch (std::exception &ex) {
            std::cerr << "Wrong y, try again!\n";
            continue;
        }
        
        if (y < 0 || y >= _p_field.height())
            std::cerr << "Wrong y, try again!\n";
        else
            break;
    }

    return std::vector({x, y});
}

void Game::make_turn()
{
    std::vector<std::size_t> x_y;
    std::size_t x;
    std::size_t y;
    
    // player turn
    if (!_bot_attack) {        
        std::cout << "\n\nPLAYER TURN!\n\n";
        std::string skill_apply;

        // save
        std::string save;
        std::cout << "Do you want to save game? [y/n] ";
        getline(std::cin, save);
        if (save == "y")
            _state.save(_p_field, _b_field, _p_skill_manager);
        
        _p_skill_manager.show();
        std::cout << "Do you want to use skill? [y/n] ";
        getline(std::cin, skill_apply);
        if (skill_apply == "y" || skill_apply == "\n") {
            bool is_skills_available = true;
                
            std::string skill_name;
            try {
                skill_name = _p_skill_manager.get_top_name();
            } catch (const NoSkillsException &ex) {
                std::cerr << ex.what() << std::endl;
                is_skills_available = false;
            }

            if (is_skills_available) {
                if (skill_name == "Scanner") {
                    x_y = input_coords();
                    x = x_y[0];
                    y = x_y[1];
                    _p_skill_manager.apply(x, y);
                } else if (skill_name == "Bombing") {
                    _p_skill_manager.apply(0, 0);
                } else
                    // due to double damage and bombing 
                    _double_damage = _p_skill_manager.apply(0, 0);
            }
        }

        std::cout << "\nAttack enemy field!\n\n";
        x_y = input_coords();
        x = x_y[0];
        y = x_y[1];

        bool destroyed = _b_field.attack(x, y);
        if (_double_damage)
            destroyed = _b_field.attack(x, y);

        if (destroyed)
            _p_skill_manager.add_random_skill();

        std::cout << "\nEnemy field:\n";
        _b_field.show();

        _bot_attack = true;

    // bot turn
    } else {
        // this implementation do some random stuff
        std::cout << "\n\nBOT TURN!\n\n";

        std::mt19937 gen(std::random_device{}());

        x = gen() % _p_field.width();
        y = gen() % _p_field.height();

        _p_field.attack(x, y);

        std::cout << "Current state of player field:\n";
        _p_field.show(false);

        _bot_attack = false;
    }
}

void Game::start_game()
{
    std::size_t width;
    std::size_t height;

    while (true) {
        if (_running)
            std::cout << "Player won in previous round, so he save all his data!\n\n";
        else {
            std::cout << "\n\nWELCOME TO NAVAL BATTLE!!!\n\n";
            
            std::string load;
            std::cout << "Do you want to load data from save? [y/n] ";
            getline(std::cin, load);
            
            if (load == "y") {
                std::cout << "\nLoading save...\n";
                _state.load(_p_field, _b_field, _p_ship_manager, _b_ship_manager, _p_skill_manager);
                std::cout << "Following configuration was loaded:\n";
                std::cout << "Player:\n";
                _p_field.show(false);
                _p_ship_manager.show();
                
                // std::cout << "Bot:\n";
                // _b_field.show(false);
                // _b_ship_manager.show();
                
                _p_skill_manager.show();
                _running = true;
            }
        }
        
        // 1. Input fields width and height
        while (!_running) {
            
            std::cout << "Input game field width and height: ";
            std::cin >> width >> height;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');            
            
            try {
                // if (!_running)
                //     _p_field = GameField(width, height);
                _p_field = GameField(width, height);
                _b_field = GameField(width, height);
                break;
            } catch (const std::exception &ex) {
                std::cerr << ex.what() << std::endl;
            }
        }

        /* Ship size   Count
         * SMALL(1)  - 4
         * MEDIUM(2) - 3
         * LARGE(3)  - 2
         * HUGE(4)   - 1
         */
        std::vector<std::size_t> ships_sizes = {1, 1, 1, 1, 2, 2, 2, 3, 3, 4};

        // 2. Initialize player and bot ShipManager's
        if (!_running) {
            _p_ship_manager = ShipManager(ships_sizes);
            _b_ship_manager = ShipManager(ships_sizes);
        
        }

        // 3. Provide player a choise to place his ships or place his ships randomly
        if (!_running) {
            std::string placement_option;

            while (true) {        
                std::cout << "Do you want to place your ships randomly? [y/n] ";
                getline(std::cin, placement_option);
                // std::cin.clear();
                // std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');            

                // random placement case
                if (placement_option == "y" || placement_option == "\n") {
                    _p_field.random_placement(_p_ship_manager);
                    _p_field.show(false);
                    // break;

                // manual placement case
                } else if (placement_option == "n") {
                    std::size_t i = 0;
                    while (i < _p_ship_manager.size()) {
                        // NOTE: switch to UI later
                        _p_field.show(false);

                        std::cout << "Currently you placing ship of size "
                                  << _p_ship_manager[i].size() << std::endl;

                        std::vector<std::size_t> x_y = input_coords();
                        std::size_t x = x_y[0];
                        std::size_t y = x_y[1];

                        bool is_vertical = false;

                        std::string orient;
                        while (true) {
                            std::cout << "Input ship orientation: [V/H] ";
                            getline(std::cin, orient);

                            if (orient == "V") {
                                is_vertical = true;
                                break;
                            } else if (orient == "H") {
                                is_vertical = false;
                                break;
                            } else
                                std::cerr << "Please, try again!\n";
                        }

                        try {
                            _p_field.place_ship(&_p_ship_manager[i], x, y, is_vertical);
                            ++i;
                        } catch (std::exception &ex) {
                            std::cerr << "Try again! " << ex.what() << std::endl;
                        }
                    }

                    // _b_ship_manager.show();
                    
                    // // 4. Place bot ships randomly
                    // _b_field.random_placement(_b_ship_manager);
                    // _b_field.show(false);
                    
                    // _p_skill_manager = std::move(SkillManager(_b_field, _b_ship_manager));
                    
                    // break;
                // wrong answer -> ask the question again
                } else {
                    std::cerr << "Aborted. Wrong option, try again!\n\n";
                    continue;
                }
                _b_ship_manager.show();
                
                // 4. Place bot ships randomly
                _b_field.random_placement(_b_ship_manager);
                _b_field.show(false);
                
                _p_skill_manager = std::move(SkillManager(_b_field, _b_ship_manager));
                
                break;
            }
        }
        
        // _b_field = GameField(width, height);
        // _b_ship_manager = ShipManager(ships_sizes);

        // // 4. Place bot ships randomly
        // _b_field.random_placement(_b_ship_manager);

        // _p_skill_manager = std::move(SkillManager(_b_field, _b_ship_manager));
  
        _running = true;

        // 5. Begin loop with player and bot turns
        while (_running)
            make_turn();

        std::string ans;

        std::cout << "Game over! Do you want to start next round? [y/n]\n";
        getline(std::cin, ans);
        if (ans == "n")
            break;
    }
}
