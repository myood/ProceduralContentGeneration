#pragma once
#include <vector>
#include <iostream>
#include "SpacePartition.h"
#include "Util.h"

struct Grid
{
    std::vector<std::vector<TILE>> tiles;
};

bool operator==(const Grid& lhs, const Grid& rhs);

::std::ostream &operator<<(::std::ostream &os, const Grid& g);

std::vector<std::vector<TILE>> createGrid(int width, int height, std::vector<SpacePartition::area_t> areas);
