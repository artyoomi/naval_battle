#include <gtest/gtest.h>

#include "../include/ShipManager.h"
#include "../include/GameField.h"

class GameFieldTest : public testing::Test {
protected:
    void SetUp() override
    {
        field1 = new GameField(10, 10);
        field2 = new GameField(GameField::min_width - 1, GameField::min_height - 1);
        field3 = new GameField(GameField::max_width + 1, GameField::max_height + 1);

        manager = new ShipManager({4});
    }

    void TearDown() override
    {
        delete field1;
        delete field2;
        delete field3;
        delete manager;
    }

    GameField   *field1;
    GameField   *field2;
    GameField   *field3;
    ShipManager *manager;
};

TEST_F(GameFieldTest, Constructor)
{

}

TEST_F(GameFieldTest, GetHeightWidth)
{
    // correct coordinates
    EXPECT_EQ(field1->width(), 10);
    EXPECT_EQ(field1->height(), 10);

    // coordinates less than minimal
    EXPECT_EQ(field2->width(), GameField::min_width);
    EXPECT_EQ(field2->height(), GameField::min_height);

    // coordinates greater than maximal
    EXPECT_EQ(field3->width(), GameField::max_width);
    EXPECT_EQ(field3->height(), GameField::max_height);
}

TEST_F(GameFieldTest, CopyConstructor)
{
    field1->place_ship(&(*manager)[0], 5, 5, true);

    GameField *field1_copy = new GameField(*field1);

    EXPECT_EQ(field1_copy->width(), field1->width());
    EXPECT_EQ(field1_copy->height(), field1->height());

    for (std::size_t x = 0; x < field1->width(); ++x) {
        for (std::size_t y = 0; y < field1->height(); ++y)
            EXPECT_EQ((*field1)[x][y], (*field1_copy)[x][y]);
    }
}

TEST_F(GameFieldTest, MoveConstructor)
{
    field1->place_ship(&(*manager)[0], 4, 4, true);

    GameField field1_moved = std::move(*field1);

    // TODO
}

TEST_F(GameFieldTest, IsShip)
{
    // TODO
}

TEST_F(GameFieldTest, PlaceShip)
{
    // TODO
}

TEST_F(GameFieldTest, Attack)
{
    // TODO
}
