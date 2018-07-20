#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <iostream>

#include "SpacePartition.h"
#include "SpatialLevelGenerator.h"
#include "Util.h"

TEST(TestSpatialLevelGenerator, rooms4squareSize10x10)
{
    const auto areas = std::vector<SpacePartition::area_t>{
        SpacePartition::area_t{ 0, 0, 4, 4 },
        SpacePartition::area_t{ 0, 4, 4, 8 },
        SpacePartition::area_t{ 4, 0, 8, 4 },
        SpacePartition::area_t{ 4, 4, 8, 8 },
    };
    const auto width = 9;
    const auto height = 9;
    const auto W = TILE::WALL_SINGLE;
    const auto F = TILE::FLOOR;
    auto expectedGrid = std::vector<std::vector<TILE>>{
        { W, W, W, W, W, W, W, W, W },
        { W, F, F, F, W, F, F, F, W },
        { W, F, F, F, W, F, F, F, W },
        { W, F, F, F, W, F, F, F, W },
        { W, W, W, W, W, W, W, W, W },
        { W, F, F, F, W, F, F, F, W },
        { W, F, F, F, W, F, F, F, W },
        { W, F, F, F, W, F, F, F, W },
        { W, W, W, W, W, W, W, W, W }
    };

    ASSERT_EQ(Grid{expectedGrid}, Grid{createGrid(width, height, areas)});
}

TEST(TestSpatialLevelGenerator, rooms2squareSize10x5_OverlappingWalls)
{
    const auto areas = std::vector<SpacePartition::area_t>{
        SpacePartition::area_t{ 0, 0, 4, 4 },
        SpacePartition::area_t{ 0, 4, 4, 9 }
    };
    const auto width = 10;
    const auto height = 5;
    const auto W = TILE::WALL_SINGLE;
    const auto F = TILE::FLOOR;
    auto expectedGrid = std::vector<std::vector<TILE>>{
        { W, W, W, W, W, W, W, W, W, W },
        { W, F, F, F, W, F, F, F, F, W },
        { W, F, F, F, W, F, F, F, F, W },
        { W, F, F, F, W, F, F, F, F, W },
        { W, W, W, W, W, W, W, W, W, W }
    };

    ASSERT_EQ(Grid{expectedGrid}, Grid{createGrid(width, height, areas)});
}

TEST(TestSpatialLevelGenerator, rooms2squareSize10x5_WallsInTheMiddle)
{
    const auto areas = std::vector<SpacePartition::area_t>{
        SpacePartition::area_t{ 1, 1, 3, 4 },
        SpacePartition::area_t{ 1, 4, 3, 8 }
    };
    const auto width = 10;
    const auto height = 5;
    const auto W = TILE::WALL_SINGLE;
    const auto F = TILE::FLOOR;
    auto expectedGrid = std::vector<std::vector<TILE>>{
        { F, F, F, F, F, F, F, F, F, F },
        { F, W, W, W, W, W, W, W, W, F },
        { F, W, F, F, W, F, F, F, W, F },
        { F, W, W, W, W, W, W, W, W, F },
        { F, F, F, F, F, F, F, F, F, F }
    };

    ASSERT_EQ(Grid{expectedGrid}, Grid{createGrid(width, height, areas)});
}

auto isNeighbour = [](const SpacePartition::area_t& a, const::SpacePartition::area_t& b)
{
    return 
        (a.top == b.bottom) or 
        (a.bottom == b.top) or
        (a.left == b.right) or 
        (a.right == b.left);
};

struct IsNeighbourFixture : testing::TestWithParam<SpacePartition::area_t> {};

TEST_P(IsNeighbourFixture, shouldReturn)
{
    const auto X = SpacePartition::area_t{ 5, 5, 7, 7};
    ASSERT_FALSE(isNeighbour(X, GetParam()));
}

    /*
      0 1 2 3 4 5 6 7 8 9 A B
    0   
    1   W W W   W W W   W W W
    2   W A W   W B W   W C W
    3   W W W   W W W   W W W
    4
    5   W W W   W W W   W W W
    6   W D W   W X W   W E W
    7   W W W   W W W   W W W
    8 
    9   W W W   W W W   W W W
    A   W F W   W G W   W H W
    B   W W W   W W W   W W W
    */
INSTANTIATE_TEST_CASE_P(SeparatedRooms, IsNeighbourFixture, ::testing::Values(
    SpacePartition::area_t{ 1, 1, 3, 3 },  //A
    SpacePartition::area_t{ 1, 5, 3, 7 },  //B
    SpacePartition::area_t{ 1, 9, 3, 11 }, //C
    SpacePartition::area_t{ 5, 1, 7, 3 },  //D
    SpacePartition::area_t{ 5, 9, 7, 11 }, //E
    SpacePartition::area_t{ 9, 1, 11, 3 }, //F
    SpacePartition::area_t{ 9, 5, 11, 7 }, //G
    SpacePartition::area_t{ 9, 9, 11, 11 } //H
));


    /*
      0 1 2 3 4 5 6 7 8 9 A
    0
    1   
    2     W W W W W W W W W
    3     W A W W B W W C W
    4     W W W W W W W W W
    5     W W W W W W W W W
    6     W D W W X W W E W
    7     W W W W W W W W W
    8     W W W W W W W W W
    9     W F W W G W W H W
    A     W W W W W W W W W
    */
INSTANTIATE_TEST_CASE_P(StickyRooms, IsNeighbourFixture, ::testing::Values(
    SpacePartition::area_t{ 2, 2, 4, 4 },  //A
    SpacePartition::area_t{ 2, 5, 4, 7 },  //B
    SpacePartition::area_t{ 2, 8, 4, 11 }, //C
    SpacePartition::area_t{ 5, 2, 8, 4 },  //D
    SpacePartition::area_t{ 5, 8, 8, 11 }, //E
    SpacePartition::area_t{ 8, 2, 11, 4 }, //F
    SpacePartition::area_t{ 8, 5, 11, 8 }, //G
    SpacePartition::area_t{ 8, 8, 11, 11 } //H
));