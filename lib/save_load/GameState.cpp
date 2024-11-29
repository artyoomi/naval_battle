#include "../../include/save_load/GameState.h"

void GameState::save(const GameField &p_field, const GameField &b_field,
                     const SkillManager &p_skill_manager) const
{
    json j;

    j["p_field"] = p_field.to_json();
    j["b_field"] = b_field.to_json();
    j["p_skill_manager"] = p_skill_manager.to_json();

    File file("save.json");
    file.open_write();
    file.write(j);
    file.close_write();
}

void GameState::load(GameField &p_field, GameField &b_field,
                     ShipManager &p_ship_manager, ShipManager &b_ship_manager,
                     SkillManager &p_skill_manager)
{
    File file(_file_name);
    file.open_read();

    json j;
    file.read(j);

    p_field.from_json(j["p_field"], p_ship_manager);
    b_field.from_json(j["b_field"], b_ship_manager);
    p_skill_manager.from_json(j["p_skill_manager"]);
}

void GameState::write_state()
{
    File file_handler(_file_name);
    file_handler.open_write();
    file_handler.write(_saved_data);
}
