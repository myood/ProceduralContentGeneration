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
#include <boost/range/adaptor/filtered.hpp>
#include <boost/range/algorithm_ext/erase.hpp>

uint SpacePartition::area_t::width() const
{
    return right - left + 1;
}

uint SpacePartition::area_t::height() const
{
    return bottom - top + 1;
}

SpacePartition::SpacePartition(SpacePartition::RandomNumberGenerator randomNumber, SpacePartition::RandomBoolGenerator randomBool)
    : randomNumber(randomNumber)
    , randomBool(randomBool)
{
}

bool SpacePartition::divide(uint desired_max_rooms, int min_room_width, int min_room_height, int space_width, int space_height)
{   
    max_rooms = desired_max_rooms;
    min_width = min_room_width;
    min_height = min_room_height;
    graph = SpacePartitioningGraph();
    roomsMap = boost::get(area_tag(), graph);
    auto root = add_vertex(graph);
    const auto wholeSpace = area_t{0, 0, space_height - 1, space_width - 1};
    boost::put(roomsMap, root, wholeSpace);

    if (not isDivisible(wholeSpace) or max_rooms <= 1)
    {
        return false;
    }

    while (true)
    {
        auto lv = leaves();
        if (lv.size() >= max_rooms)
        {
            return true;
        }
        auto l = lv | boost::adaptors::filtered([this](const auto& n){ return isDivisible(area(n)); });
        if (l.empty())
        {
            return true;
        }
        for (const auto& n : l)
        {
            if (randomBool())
            {
                divide(n);
            }
        }
    };
    return true;
}

std::vector<SpacePartition::area_t> SpacePartition::rooms()
{
    auto ls = leaves();
    std::vector<SpacePartition::area_t> rv(ls.size());
    boost::range::transform(ls, rv.begin(), [this](const auto &node) { return area(node); });
    return rv;
}

bool SpacePartition::isDivisible(const area_t area) const
{
    return (min_width <= floor(area.width() / 2) and min_height <= floor(area.height() / 2.0));
}

std::vector<SpacePartition::Node> SpacePartition::nodes()
{
    auto v_it = boost::vertices(graph);
    return std::vector<Node>(v_it.first, v_it.second);
}

std::vector<SpacePartition::Node> SpacePartition::leaves()
{
    auto ns = nodes();
    boost::remove_erase_if(ns, [this](const auto& node){
        const auto it = boost::out_edges(node, graph);
        return it.first != it.second;
    });
    return ns;
}

SpacePartition::area_t SpacePartition::area(const Node &node)
{
    return boost::get(roomsMap, node);
}

bool SpacePartition::continueDivision()
{
    return (leaves().size() < max_rooms);
}

void SpacePartition::divide(const Node &node)
{
    const auto area = boost::get(roomsMap, node);

    if (not isDivisible(area))
    {
        return;
    }

    if (randomNumber(0, 1) == 0)
    {
        const auto random_min_width = min_width;
        const auto random_max_width = area.width() - min_width;
        const auto split_width = randomNumber(random_min_width, random_max_width);
        add_child(node, area_t{area.top, area.left, area.bottom, area.left + split_width});
        add_child(node, area_t{area.top, area.left + split_width, area.bottom, area.right});
    }
    else
    {
        const auto random_min_height = min_height;
        const auto random_max_height = area.height() - min_height;
        const auto split_height = randomNumber(random_min_height, random_max_height);
        add_child(node, area_t{area.top, area.left, area.top + split_height, area.right});
        add_child(node, area_t{area.top + split_height, area.left, area.bottom, area.right});
    }
}

SpacePartition::Node SpacePartition::add_child(const Node &parent, const area_t &area)
{
    auto child = add_vertex(graph);
    boost::put(roomsMap, child, area);
    boost::add_edge(parent, child, graph);
    return child;
}

bool operator==(const SpacePartition::area_t &lhs, const SpacePartition::area_t &rhs)
{
    return lhs.top == rhs.top and lhs.bottom == rhs.bottom and lhs.left == rhs.left and lhs.right == rhs.right;
}

::std::ostream &operator<<(::std::ostream &os, const SpacePartition::area_t &area)
{
    return os << "[top: " << area.top << ", left: " << area.left << ", bottom: " << area.bottom << ", right: " << area.right << "]";
}
