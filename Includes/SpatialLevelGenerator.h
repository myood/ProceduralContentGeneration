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

std::vector<std::vector<TILE>> createGrid(int width, int height, const std::vector<SpacePartition::area_t>& areas);

std::vector<std::pair<uint, uint>> createConnections(const std::vector<SpacePartition::area_t>& areas, std::pair<uint, uint> min_max_connections_per_area);

bool isNeighbour(const SpacePartition::area_t& a, const::SpacePartition::area_t& b);
