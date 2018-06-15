#pragma once
#include "SpacePartition.h"
#include <vector>
#include <cmath>
#include <functional>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/range/iterator_range_core.hpp>
#include <boost/range/algorithm/transform.hpp>

class SpacePartition 
{
public:
    using RandomNumberGenerator = std::function<int(int min, int max)>;
    SpacePartition(RandomNumberGenerator rng);

    struct area_tag { using kind = boost::vertex_property_tag; };
    struct area_t { int top, bottom, left, right; };
    using roomsProperty = boost::property<area_tag, area_t>;
    using SpacePartitioningGraph = boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, roomsProperty>;
    using Node = boost::graph_traits<SpacePartitioningGraph>::vertex_descriptor;

    bool divide(int min_room_width, int min_room_height, int space_width, int space_height);
    std::vector<area_t> rooms();

private:
    std::vector<Node> nodes();
    area_t area(const Node& node);
    bool continueDivision(int min_room_width, int min_room_height, int space_width, int space_height);
    void divide(const Node& node);
    Node add_child(const Node& parent, const area_t& area);

    RandomNumberGenerator rng;
    SpacePartitioningGraph graph;
    boost::property_map<SpacePartitioningGraph, area_tag>::type roomsMap;
    int min_height;
    int min_width;
};

bool operator==(const SpacePartition::area_t &lhs, const SpacePartition::area_t &rhs);
::std::ostream &operator<<(::std::ostream &os, const SpacePartition::area_t &area);