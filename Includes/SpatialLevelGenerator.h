#pragma once
#include <vector>
#include <iostream>
#include <initializer_list>
#include "SpacePartition.h"
#include "Util.h"

struct Grid
{
    std::vector<std::vector<TILE>> tiles;
};

bool operator==(const Grid& lhs, const Grid& rhs);

::std::ostream &operator<<(::std::ostream &os, const Grid& g);

std::vector<std::vector<TILE>> createGrid(int width, int height, const std::vector<SpacePartition::area_t>& areas);

struct Connection
{
    uint a;
    uint b;

    Connection(std::initializer_list<uint> ab)
    {
        if (ab.size() == 2)
        {
            auto first = *std::begin(ab);
            auto last = *std::rbegin(ab);
            a = first < last ? first : last;
            b = first < last ? last : first;
        }
    }

    bool operator == (const Connection& other) const
    {
        return a == other.a and b == other.b;
    }

    bool operator != (const Connection& other) const
    {
        return not operator==(other);
    }

    bool operator < (const Connection& other) const
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
};

inline ::std::ostream &operator<<(::std::ostream &os, const Connection& c)
{
    return os << "[a: " << c.a << ", b: " << c.b << "]";
}

std::vector<Connection> createConnections(const std::vector<SpacePartition::area_t>& areas, std::pair<uint, uint> min_max_connections_per_area);

bool isNeighbour(const SpacePartition::area_t& a, const::SpacePartition::area_t& b);
