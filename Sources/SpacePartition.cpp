#include "SpacePartition.h"
#include <vector>
#include <cmath>
#include <functional>
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

SpacePartitioning::SpacePartitioning(SpacePartitioning::RandomNumberGenerator randomNumber, SpacePartitioning::RandomBoolGenerator randomBool)
    : randomNumber(randomNumber)
    , randomBool(randomBool)
{
}

bool SpacePartitioning::divide(uint desired_max_rooms, int min_room_width, int min_room_height, int space_width, int space_height)
{   
    current_rooms_size = 1;
    max_rooms = desired_max_rooms;
    min_width = min_room_width;
    min_height = min_room_height;
    graph = SpacePartition::Graph();
    roomsMap = boost::get(SpacePartition::area_tag(), graph);
    auto root = add_vertex(graph);
    const auto wholeSpace = SpacePartition::area_t{0, 0, space_height - 1, space_width - 1};
    boost::put(roomsMap, root, wholeSpace);

    if (not isDivisible(wholeSpace) or max_rooms <= 1)
    {
        return false;
    }

    while (continueDivision())
    {
        auto lv = leaves();
        auto l = lv | boost::adaptors::filtered([this](const auto& n){ return static_cast<bool>(isDivisible(area(n))); });
        if (l.empty())
        {
            return true;
        }
        for (const auto& n : l)
        {
            if (randomBool())
            {
                divide(n);
                if (not continueDivision())
                {
                    return true;
                }
            }
        }
    };
    return true;
}

SpacePartition::Areas SpacePartitioning::rooms()
{
    auto ls = leaves();
    std::vector<SpacePartition::area_t> rv(ls.size());
    boost::range::transform(ls, rv.begin(), [this](const auto &node) { return area(node); });
    return rv;
}

boost::optional<SpacePartitioning::Orientation> SpacePartitioning::isDivisible(const SpacePartition::area_t area) const
{
    auto vertically = min_width <= floor(area.width() / 2);
    auto horizontally = min_height <= floor(area.height() / 2.0);
    if (vertically and horizontally)
    {
        return {SpacePartitioning::Orientation::Both};
    }
    else if (vertically and not horizontally)
    {
        return {SpacePartitioning::Orientation::Vertical};
    }
    else if (not vertically and horizontally)
    {
        return {SpacePartitioning::Orientation::Horizontal};
    }
    else
    {
        return {};
    }
}

SpacePartition::Nodes SpacePartitioning::nodes()
{
    auto v_it = boost::vertices(graph);
    return SpacePartition::Nodes(v_it.first, v_it.second);
}

SpacePartition::Nodes SpacePartitioning::leaves()
{
    auto ns = nodes();
    boost::remove_erase_if(ns, [this](const auto& node){
        const auto it = boost::out_edges(node, graph);
        return it.first != it.second;
    });
    return ns;
}

SpacePartition::area_t SpacePartitioning::area(const SpacePartition::Node &node)
{
    return boost::get(roomsMap, node);
}

bool SpacePartitioning::continueDivision()
{
    return (num_of_rooms() < max_rooms);
}

int SpacePartitioning::num_of_rooms() const
{
    return current_rooms_size;
}

void SpacePartitioning::divide(const SpacePartition::Node &node)
{
    const auto area = boost::get(roomsMap, node);

    auto orientation = isDivisible(area);
    if (not orientation)
    {
        return;
    }

    if (orientation == Orientation::Both)
    {
        orientation = (randomNumber(0, 1) == 0 ? Orientation::Vertical : Orientation::Horizontal);
    }

    if (orientation == Orientation::Vertical)
    {
        const auto random_min_width = min_width;
        const auto random_max_width = area.width() - min_width;
        const auto split_width = randomNumber(random_min_width, random_max_width);
        add_child(node, SpacePartition::area_t{area.top, area.left, area.bottom, area.left + split_width});
        add_child(node, SpacePartition::area_t{area.top, area.left + split_width, area.bottom, area.right});
    }
    else
    {
        const auto random_min_height = min_height;
        const auto random_max_height = area.height() - min_height;
        const auto split_height = randomNumber(random_min_height, random_max_height);
        add_child(node, SpacePartition::area_t{area.top, area.left, area.top + split_height, area.right});
        add_child(node, SpacePartition::area_t{area.top + split_height, area.left, area.bottom, area.right});
    }

    current_rooms_size += 1;
}

SpacePartition::Node SpacePartitioning::add_child(const SpacePartition::Node &parent, const SpacePartition::area_t &area)
{
    auto child = add_vertex(graph);
    boost::put(roomsMap, child, area);
    boost::add_edge(parent, child, graph);
    return child;
}
