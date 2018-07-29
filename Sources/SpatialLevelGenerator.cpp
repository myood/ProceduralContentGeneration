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

Neighbourhood::Neighbourhood(index a, index b, RelativeProximity relativeProximityType)
{
    this->a = a < b ? a : b;
    this->b = a < b ? b : a;
    this->relativeProximity = relativeProximityType;
}

bool Neighbourhood::operator == (const Neighbourhood& other) const
{
    return a == other.a and b == other.b;
}

bool Neighbourhood::operator != (const Neighbourhood& other) const
{
    return not operator==(other);
}

bool Neighbourhood::operator < (const Neighbourhood& other) const
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

::std::ostream &operator<<(::std::ostream &os, const Neighbourhood& c)
{
    return os << "[a: " << c.a << ", b: " << c.b << "]";
}

Neighbourhoods getNeighbourRooms(const SpacePartition::Areas& areas)
{
    Neighbourhoods rv;

    for (uint i = 0; i < areas.size(); ++i)
    {
        for (uint j = 0; j < areas.size(); ++j)
        {
            if (i == j)
            {
                continue;
            }

            const auto& n = getRelativeProximityType(areas[i], areas[j]);
            if (n == RelativeProximity::None)
            {
                continue;
            }
            
            rv.push_back(Neighbourhood{i, j, n});
        }
    }
    boost::sort(rv);
    rv.erase(boost::end(boost::unique(rv)), rv.end());
    return rv;
}

::std::ostream &operator<<(::std::ostream &os, const RelativeProximity n)
{
    switch (n)
    {
        case RelativeProximity::None:
            os << "RelativeProximity::None";
            break;
        case RelativeProximity::A_on_the_LEFT_of_B:
            os << "RelativeProximity::A_on_the_LEFT_of_B";
            break;
        case RelativeProximity::A_on_the_RIGHT_of_B:
            os << "RelativeProximity::A_on_the_RIGHT_of_B";
            break;
        case RelativeProximity::A_on_TOP_of_B:
            os << "RelativeProximity::A_on_TOP_of_B";
            break;
        case RelativeProximity::A_UNDER_B:
            os << "RelativeProximity::A_UNDER_B";
            break;
        default:
            os << "Unsupported RelativeProximity enum value in the ostream operator";
    }
    return os;
}

RelativeProximity getRelativeProximityType(const SpacePartition::area_t& a, const::SpacePartition::area_t& b)
{
    // A under B
    if (a.top == b.bottom and a.left < b.right and a.right > b.left)
    {
        return RelativeProximity::A_UNDER_B;
    }
        // A on top of B
    if (a.bottom == b.top and a.left < b.right and a.right > b.left)
    {
        return RelativeProximity::A_on_TOP_of_B;
    }
        // A on the right of B
    if (a.left == b.right and a.top < b.bottom and a.bottom > b.top)
    {
        return RelativeProximity::A_on_the_RIGHT_of_B;
    }
        // A on the left of B
    if (a.right == b.left and a.top < b.bottom and a.bottom > b.top)
    {
        return RelativeProximity::A_on_the_LEFT_of_B;
    }

    return RelativeProximity::None;
};

SpacePartition::area_t getDoorway(const SpacePartition::area_t a, const SpacePartition::area_t b, RelativeProximity relativeProximity, uint width)
{
    const auto getHeightMidpoint = [](const SpacePartition::area_t& a){ return a.top + uint(a.height() / 2.0f); };
    const auto getWidthMidpoint = [](const SpacePartition::area_t& a){ return a.left + uint(a.width() / 2.0f); };
    switch (relativeProximity)
    {
        case RelativeProximity::A_on_the_LEFT_of_B:
            return SpacePartition::area_t{getHeightMidpoint(a), a.right, getHeightMidpoint(a), a.right};
            break;

        case RelativeProximity::A_on_the_RIGHT_of_B:
            return SpacePartition::area_t{getHeightMidpoint(a), a.left, getHeightMidpoint(a), a.left};
            break;

        case RelativeProximity::A_on_TOP_of_B:
            return SpacePartition::area_t{a.bottom, getWidthMidpoint(a), a.bottom, getWidthMidpoint(a)};
            break;
    }
    return {};
}

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

std::vector<std::vector<TILE>> createGrid(int width, int height, const SpacePartition::Areas& areas)
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