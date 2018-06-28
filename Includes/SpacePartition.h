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
#include <boost/optional.hpp>

class SpacePartition 
{
public:
    using RandomNumberGenerator = std::function<int(int min, int max)>;
    using RandomBoolGenerator = std::function<bool()>;
    SpacePartition(RandomNumberGenerator randomNumber, RandomBoolGenerator randomBool);

    struct area_tag { using kind = boost::vertex_property_tag; };
    struct area_t
    { 
        int top, left, bottom, right; 
        uint width() const;
        uint height() const;
    };
    using roomsProperty = boost::property<area_tag, area_t>;
    using SpacePartitioningGraph = boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, roomsProperty>;
    using Node = boost::graph_traits<SpacePartitioningGraph>::vertex_descriptor;

    bool divide(uint desired_max_rooms, int min_room_width, int min_room_height, int space_width, int space_height);
    std::vector<area_t> rooms();

private:
    enum class Orientation {
        Vertical,
        Horizontal,
        Both
    };
    boost::optional<Orientation> isDivisible(const area_t) const;
    std::vector<Node> nodes();
    std::vector<Node> leaves();
    area_t area(const Node& node);
    bool continueDivision();
    int num_of_rooms() const;
    void divide(const Node& node);
    Node add_child(const Node& parent, const area_t& area);

    RandomNumberGenerator randomNumber;
    RandomBoolGenerator randomBool;
    SpacePartitioningGraph graph;
    boost::property_map<SpacePartitioningGraph, area_tag>::type roomsMap;
    int min_height;
    int min_width;
    int max_rooms;
    int current_rooms_size;
};

bool operator==(const SpacePartition::area_t &lhs, const SpacePartition::area_t &rhs);
::std::ostream &operator<<(::std::ostream &os, const SpacePartition::area_t &area);