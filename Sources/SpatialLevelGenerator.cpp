#include <vector>
#include <iostream>
#include <iomanip>
#include "SpacePartition.h"
#include "SpatialLevelGenerator.h"
#include "Util.h"
#include <boost/algorithm/string/join.hpp>
#include <boost/range/algorithm/unique.hpp>
#include <boost/range/algorithm/sort.hpp>
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

Connection::Connection(std::initializer_list<uint> ab)
{
    if (ab.size() == 2)
    {
        auto first = *std::begin(ab);
        auto last = *std::rbegin(ab);
        a = first < last ? first : last;
        b = first < last ? last : first;
    }
}

bool Connection::operator == (const Connection& other) const
{
    return a == other.a and b == other.b;
}

bool Connection::operator != (const Connection& other) const
{
    return not operator==(other);
}

bool Connection::operator < (const Connection& other) const
{
    if (a < other.a)
    {
        return true;
    }
    else if (other.a < a)
    {
        return false;
    }
    else if (b < other.b)
    {
        return true;
    }
    else
    {
        return false;
    }
}

::std::ostream &operator<<(::std::ostream &os, const Connection& c)
{
    return os << "[a: " << c.a << ", b: " << c.b << "]";
}

std::vector<Connection> createConnections(const std::vector<SpacePartition::area_t>& areas)
{
    std::vector<Connection> rv;
    for (uint i = 0; i < areas.size(); ++i)
    {
        for (uint j = 0; j < areas.size(); ++j)
        {
            if (i == j)
            {
                continue;
            }

            if (isNeighbour(areas[i], areas[j]))
            {
                rv.push_back(Connection{i, j});
            }
        }
    }
    boost::sort(rv);
    rv.erase(boost::end(boost::unique(rv)), rv.end());
    return rv;
}

bool isNeighbour(const SpacePartition::area_t& a, const::SpacePartition::area_t& b)
{
    return
        // B over A
        (a.top == b.bottom and a.left < b.right and a.right > b.left) or 
        // B under A
        (a.bottom == b.top and a.left < b.right and a.right > b.left) or
        // B left of A
        (a.left == b.right and a.top < b.bottom and a.bottom > b.top) or 
        // B right of A
        (a.right == b.left and a.top < b.bottom and a.bottom > b.top);
};

