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

    bool divide(int min_room_width, int min_room_height, int space_width, int space_height)
    {
        min_width = min_room_width;
        min_height = min_room_height;
        graph = SpacePartitioningGraph();
        areaCoordsMap = boost::get(area_coords_t(), graph);
        auto root = add_vertex(graph);
        boost::put(areaCoordsMap, root, area_coords{0, space_height, 0, space_width});
    }

    auto nodes()
    {
        auto v_it = boost::vertices(graph);
        return std::vector<Node>(v_it.first, v_it.second);
    }

private:
    void divide(auto node)
    {
        const auto areaCoord = boost::get(areaCoordsMap, node);
        const auto width = areaCoord.right - areaCoord.left;
        const auto height = areaCoord.bottom - areaCoord.top;
        const auto random_min_width = min_width;
        const auto random_max_width = width - min_width;
    }

    void add_child(auto parent, auto child)
    {
        boost::add_edge(std::make_pair(parent, child), graph);
    }

    SpacePartitioningGraph graph;
    boost::property_map<SpacePartitioningGraph, area_coords_t>::type areaCoordsMap;
    int min_height;
    int min_width;
};

TEST_F(TestSpatialLevelGenerator, envTest)
{
    SpacePartition sp;
    const auto min_height = 10;
    const auto min_width = 10;
    const auto height = 20;
    const auto width = 20;
    ASSERT_TRUE(sp.divide(min_width, min_height, width, height));
    ASSERT_EQ(3, sp.nodes().size());
}