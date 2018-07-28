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

enum class Neighbourhood
{
    None,
    A_on_the_LEFT_of_B,
    A_on_the_RIGHT_of_B,
    A_on_TOP_of_B,
    A_UNDER_B
};

::std::ostream &operator<<(::std::ostream &os, const Neighbourhood n);

Neighbourhood getNeighbourhoodType(const SpacePartition::area_t& a, const::SpacePartition::area_t& b);

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