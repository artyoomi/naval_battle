#include <gtest/gtest.h>

#include "../include/Ship.h"

class ShipTest : public testing::Test {
protected:
    void SetUp() override
    {
        ship1 = new Ship(3);
        ship2 = new Ship(1);
        ship3 = new Ship(4);
    }

    void TearDown() override
    {
        delete ship1;
        delete ship2;
        delete ship3;
    }

    Ship *ship1;
    Ship *ship2;
    Ship *ship3;
};

TEST_F(ShipTest, Constructor)
{
    EXPECT_THROW({ Ship(5); }, std::invalid_argument);
    EXPECT_THROW({ Ship(0); }, std::invalid_argument);

    EXPECT_EQ(ship1->size(), 3);
}

TEST_F(ShipTest, IndexingOperator)
{
    EXPECT_THROW({ (*ship1)[-1]; }, std::out_of_range);
    EXPECT_EQ((*ship1)[ 0], Ship::SegState::INTACT);
    EXPECT_EQ((*ship1)[ 1], Ship::SegState::INTACT);
    EXPECT_EQ((*ship1)[ 2], Ship::SegState::INTACT);
    EXPECT_THROW({ (*ship1)[ 3]; }, std::out_of_range);
}

TEST_F(ShipTest, GetSize)
{
    EXPECT_EQ(ship1->size(), 3);
    EXPECT_EQ(ship2->size(), 1);
    EXPECT_EQ(ship3->size(), 4);
}

TEST_F(ShipTest, GetSegs)
{
    EXPECT_EQ(ship1->segs(), std::vector({Ship::SegState::INTACT,
                                          Ship::SegState::INTACT,
                                          Ship::SegState::INTACT}));
    EXPECT_EQ(ship2->segs(), std::vector({Ship::SegState::INTACT}));
    EXPECT_EQ(ship3->segs(), std::vector({Ship::SegState::INTACT,
                                          Ship::SegState::INTACT,
                                          Ship::SegState::INTACT,
                                          Ship::SegState::INTACT}));
}

TEST_F(ShipTest, GetHealth)
{
    EXPECT_EQ(ship1->health(), 6);
    EXPECT_EQ(ship2->health(), 2);
    EXPECT_EQ(ship3->health(), 8);
}

TEST_F(ShipTest, Orientation)
{
    EXPECT_EQ(ship1->vertical(), false);
    ship2->set_vertical();
    EXPECT_EQ(ship2->vertical(), true);
    EXPECT_EQ(ship3->vertical(), false);
}

TEST_F(ShipTest, TakeDamage)
{
    EXPECT_THROW({ship1->take_damage(5); }, std::out_of_range);
    ship1->take_damage(1);
    ship1->take_damage(0);
    EXPECT_EQ(ship1->segs(), std::vector({Ship::SegState::DAMAGED,
                                          Ship::SegState::DAMAGED,
                                          Ship::SegState::INTACT}));
}
