#include "../include/SkillManager.h"

#include <random>
#include <algorithm>

SkillManager::SkillManager() noexcept
{
    _skills.emplace_back(DOUBLE_DAMAGE, SCANNER, BOMBING);

    std::random_device rd_dev;
    std::mt19937 rd_gen(rd_dev());
    std::shuffle(_skills.begin(), _skills.end(), rd_gen);
}

void SkillManager::get_random_skill() noexcept
{
    std::random_device rd_dev;
    std::mt19937 rd_gen(rd_dev());
    std::uniform_int_distribution<> rd_distr(0, 2);

    switch (rd_distr(rd_gen)) {
        case 0:
            _skills.emplace_back(DOUBLE_DAMAGE);
            break;
        case 1:
            _skills.emplace_back(SCANNER);
            break;
        case 2:
            _skills.emplace_back(BOMBING);
            break;
    }
}
