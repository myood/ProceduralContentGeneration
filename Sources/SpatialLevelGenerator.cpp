#include <vector>
#include <iostream>
#include "SpacePartition.h"
#include "SpatialLevelGenerator.h"
#include "Util.h"
#include <boost/algorithm/string/join.hpp>
#include <boost/range/adaptor/transformed.hpp>

bool operator==(const Grid& lhs, const Grid& rhs)
{
    return lhs.tiles == rhs.tiles;
}

::std::ostream &operator<<(::std::ostream &os, const Grid& g)
{
    auto to_str = [](auto t)
    {
        std::ostringstream out;
        out << std::setw(2) << static_cast<int>(t);
        return out.str();
    };

    os << std::endl;
    for (const auto& row : g.tiles)
    {
        os << "[" << boost::algorithm::join(
            row | boost::adaptors::transformed(to_str), "][") << "]" << std::endl;
    }
    return os;
}

std::vector<std::vector<TILE>> createGrid(int width, int height, const std::vector<SpacePartition::area_t>& areas)
{ 
    auto grid = std::vector<std::vector<TILE>>(height, std::vector<TILE>(width, TILE::FLOOR));
    for (const auto& a : areas)
    {
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

std::vector<std::pair<uint, uint>> createConnections(const std::vector<SpacePartition::area_t>& areas, std::pair<uint, uint> min_max_connections_per_area)
{
    std::vector<std::pair<uint, uint>> retval;
    for (int i = 0; i < areas.size() - 1; ++i)
    {
        retval.push_back(std::make_pair(i, i + 1));
    }
    return retval;
}

bool isNeighbour(const SpacePartition::area_t& a, const::SpacePartition::area_t& b)
{
    return 
        // B over A
        (a.top == b.bottom and a.left <= b.right and a.right >= b.left) or 
        // B under A
        (a.bottom == b.top and a.left <= b.right and a.right >= b.left) or
        // B left of A
        (a.left == b.right and a.top <= b.bottom and a.bottom >= b.top) or 
        // B right of A
        (a.right == b.left and a.top <= b.bottom and a.bottom >= b.top);
};

