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

using RandomNumberGenerator = std::function<int(int min, int max)>;
SpacePartition::SpacePartition(RandomNumberGenerator rng)
    : rng(rng)
{
}

bool SpacePartition::divide(int min_room_width, int min_room_height, int space_width, int space_height)
{
    if (not continueDivision(min_room_width, min_room_height, space_width, space_height))
    {
        return false;
    }
    min_width = min_room_width;
    min_height = min_room_height;
    graph = SpacePartitioningGraph();
    areasMap = boost::get(area_tag(), graph);
    auto root = add_vertex(graph);
    boost::put(areasMap, root, area_t{0, space_height, 0, space_width});
    divide(root);
    return true;
}

std::vector<SpacePartition::area_t> SpacePartition::areas()
{
    const auto ns = nodes();
    std::vector<SpacePartition::area_t> rv(ns.size());
    boost::range::transform(ns, rv.begin(), [this](const auto &node) { return area(node); });
    return rv;
}

std::vector<SpacePartition::Node> SpacePartition::nodes()
{
    auto v_it = boost::vertices(graph);
    return std::vector<Node>(v_it.first, v_it.second);
}

SpacePartition::area_t SpacePartition::area(const Node &node)
{
    return boost::get(areasMap, node);
}

bool SpacePartition::continueDivision(int min_room_width, int min_room_height, int space_width, int space_height)
{
    return min_room_width <= floor(space_width / 2) and min_room_height <= floor(space_height / 2.0);
}

void SpacePartition::divide(const Node &node)
{
    const auto area = boost::get(areasMap, node);
    const auto width = area.right - area.left;
    const auto height = area.bottom - area.top;

    if (not continueDivision(min_width, min_height, width, height))
    {
        return;
    }

    if (rng(0, 1) == 0)
    {
        const auto random_min_width = min_width;
        const auto random_max_width = width - min_width;
        const auto split_width = rng(random_min_width, random_max_width);
        divide(
            add_child(node, area_t{area.top, area.bottom, area.left, area.left + split_width}));
        divide(
            add_child(node, area_t{area.top, area.bottom, area.left + split_width + 1, area.right}));
    }
    else
    {
        const auto random_min_height = min_height;
        const auto random_max_height = height - min_height;
        const auto split_height = rng(random_min_height, random_max_height);
        divide(
            add_child(node, area_t{area.top, area.top + split_height, area.left, area.right}));
        divide(
            add_child(node, area_t{area.top + split_height + 1, area.bottom, area.left, area.right}));
    }
}

SpacePartition::Node SpacePartition::add_child(const Node &parent, const area_t &area)
{
    auto child = add_vertex(graph);
    boost::put(areasMap, child, area);
    boost::add_edge(parent, child, graph);
    return child;
}

bool operator==(const SpacePartition::area_t &lhs, const SpacePartition::area_t &rhs)
{
    return lhs.top == rhs.top and lhs.bottom == rhs.bottom and lhs.left == rhs.left and lhs.right == lhs.right;
}

::std::ostream &operator<<(::std::ostream &os, const SpacePartition::area_t &area)
{
    return os << "[top: " << area.top << ", bottom: " << area.bottom << ", left: " << area.left << ", right: " << area.right << "]";
}
