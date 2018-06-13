#include <vector>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/range/iterator_range_core.hpp>

using namespace testing;

struct TestSpatialLevelGenerator : public Test
{

};

class SpacePartition 
{
public:
    struct area_coords_t { using kind = boost::vertex_property_tag; };
    struct area_coords { int top, bottom, left, right; };
    using AreaCoordsProperty = boost::property<area_coords_t, area_coords>;
    using SpacePartitioningGraph = boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, AreaCoordsProperty>;
    using Node = boost::graph_traits<SpacePartitioningGraph>::vertex_descriptor;

    SpacePartition(int min_width, int min_height)
    : min_width(min_width)
    , min_height(min_height)
    {}

    void divide(int space_wdth, int space_height)
    {

    }

    auto nodes()
    {
        auto v_it = boost::vertices(graph);
        return std::vector<Node>(v_it.first, v_it.second);
    }

private:
    void add_child(auto parent, auto child)
    {
        boost::add_edge(std::make_pair(parent, child), graph);
    }

    SpacePartitioningGraph graph;
    boost::property_map<SpacePartitioningGraph, area_coords_t>::type areaCoordMap;
    const int min_height;
    const int min_width;
};

TEST_F(TestSpatialLevelGenerator, envTest)
{
    const auto min_height = 10;
    const auto min_width = 10;
    SpacePartition sp(min_width, min_height);
    const auto height = 20;
    const auto width = 20;
    sp.divide(width, height);
    ASSERT_EQ(3, sp.nodes().size());
}