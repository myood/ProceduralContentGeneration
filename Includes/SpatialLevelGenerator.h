#pragma once
#include <vector>
#include <iostream>
#include <initializer_list>
#include "SpacePartition.h"
#include "Util.h"

enum class RelativeProximity
{
    None,
    A_on_the_LEFT_of_B,
    A_on_the_RIGHT_of_B,
    A_on_TOP_of_B,
    A_UNDER_B
};

::std::ostream &operator<<(::std::ostream &os, const RelativeProximity n);

RelativeProximity getRelativeProximityType(const SpacePartition::area_t& a, const::SpacePartition::area_t& b);

struct Neighbourhood
{
    using index = uint;
    index a;
    index b;
    RelativeProximity relativeProximity;

    Neighbourhood(index a, index b, RelativeProximity);
    bool operator == (const Neighbourhood& other) const;
    bool operator != (const Neighbourhood& other) const;
    bool operator < (const Neighbourhood& other) const;
};

::std::ostream &operator<<(::std::ostream &os, const Neighbourhood& c);

using Neighbourhoods = std::vector<Neighbourhood>;
Neighbourhoods getNeighbourRooms(const SpacePartition::Areas& areas);

struct Grid
{
    std::vector<std::vector<TILE>> tiles;
};

bool operator==(const Grid& lhs, const Grid& rhs);

::std::ostream &operator<<(::std::ostream &os, const Grid& g);

std::vector<std::vector<TILE>> createGrid(int width, int height, const SpacePartition::Areas& areas);

SpacePartition::area_t getDoorway(const SpacePartition::area_t a, const SpacePartition::area_t b, RelativeProximity, uint width);
