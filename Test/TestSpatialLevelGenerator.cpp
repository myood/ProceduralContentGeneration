#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "SpacePartition.h"
#include "Util.h"

struct Grid
{
    std::vector<std::vector<TILE>> tiles;
};

bool operator==(const Grid& lhs, const Grid& rhs)
{
    return lhs.tiles == rhs.tiles;
}

::std::ostream &operator<<(::std::ostream &os, const Grid& g)
{
    for (const auto& row : g.tiles)
    {
        for (const auto& t : row)
        {
            os << " " << static_cast<int>(t) << " ";
        }
        os << std::endl;
    }
    return os;
}

TEST(TestSpatialLevelGenerator, shouldRecreateSize)
{
    auto createGrid = [](int width, int height, std::vector<SpacePartition::area_t> areas)
    { 
        auto grid = std::vector<std::vector<TILE>>(width, std::vector<TILE>(height, TILE::FLOOR));
        for (const auto& a : areas)
        {
            //top 5, bottom 10, left 3, right 7
            for (int i = a.top; i <= a.bottom; ++i)
            {
                grid[i][a.left] = TILE::WALL_SINGLE;
                grid[i][a.right] = TILE::WALL_SINGLE;
            }

            for (int j = a.left; j <= a.right; ++j)
            {
                grid[a.top][j] = TILE::WALL_SINGLE;
                grid[a.bottom][j] = TILE::WALL_SINGLE;
            }
        }
        return grid;
    };
    const auto areas = std::vector<SpacePartition::area_t>{
        SpacePartition::area_t{ 0, 9, 0, 9 },
        SpacePartition::area_t{ 0, 10, 10, 19 },
        SpacePartition::area_t{ 10, 19, 0, 9 },
        SpacePartition::area_t{ 10, 19, 10, 19 },
    };
    const auto width = 20;
    const auto height = 20;
    const auto W = TILE::WALL_SINGLE;
    const auto F = TILE::FLOOR;
    auto expectedGrid = std::vector<std::vector<TILE>>(20, std::vector<TILE>(20, F));
    for (int i = 0; i < expectedGrid.size(); ++i)
    {
        expectedGrid[i][0] = W;
        expectedGrid[i][expectedGrid.size() - 1] = W;
        expectedGrid[i][9] = W;
        expectedGrid[i][10] = W;

        expectedGrid[0][i] = W;
        expectedGrid[expectedGrid.size() - 1][i] = W;
        expectedGrid[9][i] = W;
        expectedGrid[10][i] = W;
    }

    ASSERT_EQ(Grid{expectedGrid}, Grid{createGrid(width, height, areas)});
}