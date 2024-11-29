#include "../../include/skills/Bombing.h"

#include <random>

bool Bombing::apply(GameField& field, ShipManager& manager, std::size_t x, std::size_t y)
{
    // search for all alive segments
    std::vector<std::pair<std::size_t, std::size_t>> alive_segs;
    for (std::size_t ship_i = 0; ship_i < manager.size(); ++ship_i) {
        if (manager[ship_i].health()) {
            for (std::size_t seg_i = 0; seg_i < manager[ship_i].size(); ++seg_i) {
                if (manager[ship_i][seg_i] != Ship::SegState::DESTROYED)
                    alive_segs.emplace_back(std::make_pair(ship_i, seg_i));
            }
        }
    }

    if (alive_segs.size() == 0)
        return false;

    std::random_device rd_dev;
    std::mt19937 rd_gen(rd_dev());
    std::uniform_int_distribution<> rd_distr(0, 200);

    std::size_t rd_num = rd_distr(rd_gen);

    std::pair ind_pair = alive_segs[rd_num % alive_segs.size()];

    manager[ind_pair.first].take_damage(ind_pair.second);

    return true;
}

std::string Bombing::name() { return "Bombing"; }
