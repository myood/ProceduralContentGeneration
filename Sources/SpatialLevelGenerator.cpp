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

ConnectedRooms::ConnectedRooms(index a, index b, Neighbourhood neighbourhoodType)
{
    this->a = a < b ? a : b;
    this->b = a < b ? b : a;
    this->neighbourhood = neighbourhoodType;
}

bool ConnectedRooms::operator == (const ConnectedRooms& other) const
{
    return a == other.a and b == other.b;
}

bool ConnectedRooms::operator != (const ConnectedRooms& other) const
{
    return not operator==(other);
}

bool ConnectedRooms::operator < (const ConnectedRooms& other) const
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

::std::ostream &operator<<(::std::ostream &os, const ConnectedRooms& c)
{
    return os << "[a: " << c.a << ", b: " << c.b << "]";
}

std::vector<ConnectedRooms> createConnectedRooms(const std::vector<SpacePartition::area_t>& areas)
{
    std::vector<ConnectedRooms> rv;

    for (uint i = 0; i < areas.size(); ++i)
    {
        for (uint j = 0; j < areas.size(); ++j)
        {
            if (i == j)
            {
                continue;
            }

            const auto& n = getNeighbourhoodType(areas[i], areas[j]);
            if (n == Neighbourhood::None)
            {
                continue;
            }
            
            rv.push_back(ConnectedRooms{i, j, n});
        }
    }
    boost::sort(rv);
    rv.erase(boost::end(boost::unique(rv)), rv.end());
    return rv;
}

::std::ostream &operator<<(::std::ostream &os, const Neighbourhood n)
{
    switch (n)
    {
        case Neighbourhood::None:
            os << "Neighbourhood::None";
            break;
        case Neighbourhood::A_on_the_LEFT_of_B:
            os << "Neighbourhood::A_on_the_LEFT_of_B";
            break;
        case Neighbourhood::A_on_the_RIGHT_of_B:
            os << "Neighbourhood::A_on_the_RIGHT_of_B";
            break;
        case Neighbourhood::A_on_TOP_of_B:
            os << "Neighbourhood::A_on_TOP_of_B";
            break;
        case Neighbourhood::A_UNDER_B:
            os << "Neighbourhood::A_UNDER_B";
            break;
        default:
            os << "Unsupported Neighbourhood enum value in the ostream operator";
    }
    return os;
}

Neighbourhood getNeighbourhoodType(const SpacePartition::area_t& a, const::SpacePartition::area_t& b)
{
    // A under B
    if (a.top == b.bottom and a.left < b.right and a.right > b.left)
    {
        return Neighbourhood::A_UNDER_B;
    }
        // A on top of B
    if (a.bottom == b.top and a.left < b.right and a.right > b.left)
    {
        return Neighbourhood::A_on_TOP_of_B;
    }
        // A on the right of B
    if (a.left == b.right and a.top < b.bottom and a.bottom > b.top)
    {
        return Neighbourhood::A_on_the_RIGHT_of_B;
    }
        // A on the left of B
    if (a.right == b.left and a.top < b.bottom and a.bottom > b.top)
    {
        return Neighbourhood::A_on_the_LEFT_of_B;
    }

    return Neighbourhood::None;
};

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
