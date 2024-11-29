#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <string>
#include <nlohmann/json.hpp>

#include "../GameField.h"
#include "../SkillManager.h"
#include "../save_load/File.h"

using json = nlohmann::json;

/* Format of file with save:
 * 1. Ships and their coordinates;
 * 2. Field and states of cells
 * 3. 
 *
 *
 *
 *
 *
 */

class GameState {
public:
    GameState(std::string file_name) : _file_name{file_name} {}
    
    void save(const GameField &p_field, const GameField &b_field,
              const SkillManager &p_skill_manager) const;
    void load(GameField &p_field, GameField &b_field,
              ShipManager &p_ship_manager, ShipManager &b_ship_manager,
              SkillManager &p_skill_manager);    

    friend std::ostream& operator<<(std::ostream& os, const GameState& game_state) {
        File file_handler(game_state._file_name);
        file_handler.open_read();

        json j;
        file_handler.read(j);
        
        os << j.dump(4) << std::endl;
        
        return os;
    }

    friend std::istream& operator>>(std::istream& is, GameState& game_state) {
        json j;
        
        is >> j; 
        game_state._saved_data = j;
        if (j.is_null()) {
            throw std::runtime_error("Failed to read valid JSON data.");
        }
        
        game_state.write_state();
        return is;
    }

    void write_state();
    
private:
    std::string _file_name;
    json _saved_data;
};

#endif
