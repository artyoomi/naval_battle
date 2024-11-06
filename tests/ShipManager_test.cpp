#include <gtest/gtest.h>

#include "../include/ShipManager.h"
#include "../include/GameField.h"

class ShipManagerTest : public testing::Test {
protected:
    void SetUp() override
    {
        manager = new ShipManager({1, 2, 3, 4, 3, 2, 2, 4});
    }

    void TearDown() override
    {
        delete manager;
    }

    ShipManager *manager;
};

TEST_F(ShipManagerTest, Constructor)
{
    std::vector<std::size_t> correct_sizes = {1, 2, 3, 4, 3, 2, 2, 4};
    for (std::size_t i = 0; i < manager->inactive_size(); ++i)
        EXPECT_EQ(manager->inactive_ship(i).size(), correct_sizes[i]);
}

TEST_F(ShipManagerTest, GetSize)
{

}
