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
        SpacePartition::area_t{ 0, 5, 4, 9 },
        SpacePartition::area_t{ 5, 0, 9, 4 },
        SpacePartition::area_t{ 5, 5, 9, 9 },
    };
    const auto width = 10;
    const auto height = 10;
    const auto W = TILE::WALL_SINGLE;
    const auto F = TILE::FLOOR;
    auto expectedGrid = std::vector<std::vector<TILE>>{
        { W, W, W, W, W, W, W, W, W, W },
        { W, F, F, F, W, W, F, F, F, W },
        { W, F, F, F, W, W, F, F, F, W },
        { W, F, F, F, W, W, F, F, F, W },
        { W, W, W, W, W, W, W, W, W, W },
        { W, W, W, W, W, W, W, W, W, W },
        { W, F, F, F, W, W, F, F, F, W },
        { W, F, F, F, W, W, F, F, F, W },
        { W, F, F, F, W, W, F, F, F, W },
        { W, W, W, W, W, W, W, W, W, W }
    };

    ASSERT_EQ(Grid{expectedGrid}, Grid{createGrid(width, height, areas)});
}

TEST(TestSpatialLevelGenerator, rooms2squareSize10x5)
{
    const auto areas = std::vector<SpacePartition::area_t>{
        SpacePartition::area_t{ 0, 0, 4, 4 },
        SpacePartition::area_t{ 0, 5, 4, 9 }
    };
    const auto width = 10;
    const auto height = 5;
    const auto W = TILE::WALL_SINGLE;
    const auto F = TILE::FLOOR;
    auto expectedGrid = std::vector<std::vector<TILE>>{
        { W, W, W, W, W, W, W, W, W, W },
        { W, F, F, F, W, W, F, F, F, W },
        { W, F, F, F, W, W, F, F, F, W },
        { W, F, F, F, W, W, F, F, F, W },
        { W, W, W, W, W, W, W, W, W, W }
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

TEST(TestSpatialLevelGenerator, createConnections)
{
    
}