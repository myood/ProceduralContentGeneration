#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/properties.hpp>

using namespace testing;

struct TestSpatialLevelGenerator : public Test
{

};

struct area_coords_t {
    typedef boost::vertex_property_tag kind;
};
struct area_coords {
    int top;
    int left;
    int bottom;
    int right;
};
using AreaCoordsProperty = boost::property<area_coords_t, area_coords>;
using SpacePartitioningGraph = boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, AreaCoordsProperty>;

void divide(auto& areaCoordsMap, auto& g, auto& vertex, int min_width, int min_height)
{
    auto areaCoords = boost::get(areaCoordsMap, vertex, g);
    auto width = areaCoords.right - areaCoords.left;
    auto height = areaCoords.bottom - areaCoords.bottom;
    if (width >= min_width and height >= min_height)
    {

    }
}

auto add_vertex(auto& areaCoordsMap, auto& g, auto& areaCoords)
{
    auto vertex = boost::add_vertex(g);
    boost::put(areaCoordsMap, vertex, area_coords{0, 0, 20, 20});
    return vertex;
}

TEST_F(TestSpatialLevelGenerator, envTest)
{
    SpacePartitioningGraph g;
    auto areaCoordsMap = boost::get(area_coords_t(), g);
    const auto min_height = 10;
    const auto min_width = 10;
    divide(areaCoordsMap, g,
        add_vertex(areaCoordsMap, g, area_coords{0, 0, 20, 20}),
        min_height,
        min_width);

}