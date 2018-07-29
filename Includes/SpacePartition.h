#pragma once
#include "SpacePartition.h"
#include <vector>
#include <cmath>
#include <functional>
#include <boost/range/iterator_range_core.hpp>
#include <boost/range/algorithm/transform.hpp>
#include <boost/optional.hpp>
#include "SpacePartition/Graph.h"

class SpacePartitioning
{
public:
    using RandomNumberGenerator = std::function<int(int min, int max)>;
    using RandomBoolGenerator = std::function<bool()>;
    SpacePartitioning(RandomNumberGenerator randomNumber, RandomBoolGenerator randomBool);

    bool divide(uint desired_max_rooms, uint min_room_width, uint min_room_height, uint space_width, uint space_height);
    std::vector<SpacePartition::area_t> rooms();

private:
    enum class Orientation {
        Vertical,
        Horizontal,
        Both
    };
    boost::optional<Orientation> isDivisible(const SpacePartition::area_t) const;
    SpacePartition::Nodes nodes();
    SpacePartition::Nodes leaves();
    SpacePartition::area_t area(const SpacePartition::Node& node);
    bool continueDivision();
    int num_of_rooms() const;
    void divide(const SpacePartition::Node& node);
    SpacePartition::Node add_child(const SpacePartition::Node& parent, const SpacePartition::area_t& area);

    RandomNumberGenerator randomNumber;
    RandomBoolGenerator randomBool;
    SpacePartition::Graph graph;
    SpacePartition::GraphRooms roomsMap;
    int min_height;
    int min_width;
    int max_rooms;
    int current_rooms_size;
};