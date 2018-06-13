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

TEST_F(TestSpatialLevelGenerator, envTest)
{
    SpacePartitioningGraph g;
}