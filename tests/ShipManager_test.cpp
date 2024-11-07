#include <gtest/gtest.h>

#include "../include/ShipManager.h"
#include "../include/GameField.h"

class ShipManagerTest : public testing::Test {
protected:
    void SetUp() override
    {
        manager1 = new ShipManager({1, 2, 3, 4, 3, 2, 2, 4});
        manager2 = new ShipManager({});
    }

    void TearDown() override
    {
        delete manager1;
        delete manager2;
    }

    ShipManager *manager1;
    ShipManager *manager2;
};

TEST_F(ShipManagerTest, Constructor)
{
    std::vector<std::size_t> correct_sizes = {1, 2, 3, 4, 3, 2, 2, 4};
    for (std::size_t i = 0; i < manager1->size(); ++i)
        EXPECT_EQ((*manager1)[i].size(), correct_sizes[i]);
}

TEST_F(ShipManagerTest, GetSize)
{
    EXPECT_EQ(manager1->size(), 8);
    EXPECT_EQ(manager2->size(), 0);
}

TEST_F(ShipManagerTest, IndexingOperator)
{
    EXPECT_EQ((*manager1)[0].size(), 1);
    EXPECT_EQ((*manager1)[manager1->size() - 1].size(), 4);

    EXPECT_THROW({ (*manager1)[-1]; }, std::out_of_range);
    EXPECT_THROW({ (*manager1)[manager1->size()].size(); }, std::out_of_range);
}
