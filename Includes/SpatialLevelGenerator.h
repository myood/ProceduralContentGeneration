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

    Connection(std::initializer_list<uint> ab);
    bool operator == (const Connection& other) const;
    bool operator != (const Connection& other) const;
    bool operator < (const Connection& other) const;
};

::std::ostream &operator<<(::std::ostream &os, const Connection& c);

std::vector<Connection> createConnections(const std::vector<SpacePartition::area_t>& areas);

bool isNeighbour(const SpacePartition::area_t& a, const::SpacePartition::area_t& b);
