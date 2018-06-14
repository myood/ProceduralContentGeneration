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

using namespace testing;

struct TestSpatialLevelGenerator : public Test
{

};

class SpacePartition 
{
public:
    using RandomNumberGenerator = std::function<int(int min, int max)>;
    SpacePartition(RandomNumberGenerator rng)
    : rng(rng)
    {}

    struct area_tag { using kind = boost::vertex_property_tag; };
    struct area_t { int top, bottom, left, right; };
    using areasProperty = boost::property<area_tag, area_t>;
    using SpacePartitioningGraph = boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, areasProperty>;
    using Node = boost::graph_traits<SpacePartitioningGraph>::vertex_descriptor;

    bool divide(int min_room_width, int min_room_height, int space_width, int space_height)
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

    auto nodes()
    {
        auto v_it = boost::vertices(graph);
        return std::vector<Node>(v_it.first, v_it.second);
    }

    area_t area(const Node& node)
    {
        return boost::get(areasMap, node);
    }

private:
    bool continueDivision(int min_room_width, int min_room_height, int space_width, int space_height)
    {
        return 
            min_room_width <= floor(space_width / 2) 
            and min_room_height <= floor(space_height / 2.0);
    }

    void divide(const Node& node)
    {
        const auto area = boost::get(areasMap, node);
        const auto width = area.right - area.left;
        const auto height = area.bottom - area.top;
        
        if (not continueDivision(min_width, min_height, width, height))
        {
            return;
        }

        auto split_width = width;
        auto split_height = height;
        int split_orientation = rng(0, 1);
        if (split_orientation == 0)
        {
            const auto random_min_width = min_width;
            const auto random_max_width = width - min_width;
            split_width = rng(random_min_width, random_max_width);
        }
        else
        {
            const auto random_min_height = min_height;
            const auto random_max_height = height - min_height;
            split_height = rng(random_min_height, random_max_height);
        }


        divide(
            add_child(node, area_t{area.top, area.top + split_height, area.left, area.left + split_width}));
        divide(
            add_child(node, area_t{area.top + split_height, area.bottom, area.left + split_width, area.bottom}));
    }

    Node add_child(const Node& parent, const area_t& area)
    {
        auto child = add_vertex(graph);
        boost::put(areasMap, child, area);
        boost::add_edge(parent, child, graph);
        return child;
    }

    RandomNumberGenerator rng;
    SpacePartitioningGraph graph;
    boost::property_map<SpacePartitioningGraph, area_tag>::type areasMap;
    int min_height;
    int min_width;
};

    struct RngMock
    {
        MOCK_METHOD2(generate, int(int, int));
        auto getFunction() { return [this](int min, int max){ return generate(min, max); }; }
    };

static bool operator!=(const SpacePartition::area_t& a1, const SpacePartition::area_t& a2) {
    return true;
}

TEST_F(TestSpatialLevelGenerator, doesNotAcceptMinDimensionGreaterThenHalfOfThatDimension)
{
    SpacePartition sp{SpacePartition::RandomNumberGenerator()};
    const auto min_height = 2;
    const auto min_width = 2;
    const auto height = 3;
    const auto width = 3;
    ASSERT_FALSE(sp.divide(min_width, min_height, width, height));
}

TEST_F(TestSpatialLevelGenerator, envTest)
{
    RngMock rngMock;
    EXPECT_CALL(rngMock, generate(0, 1)).WillRepeatedly(testing::Return(0));
    EXPECT_CALL(rngMock, generate(10/*min*/, 10/*max*/)).WillRepeatedly(testing::Return(10));
    SpacePartition sp{rngMock.getFunction()};
    const auto min_height = 10;
    const auto min_width = 10;
    const auto height = 20;
    const auto width = 20;
    const auto begin = 0;
    ASSERT_TRUE(sp.divide(min_width, min_height, width, height));
    const auto size = 3;
    ASSERT_EQ(size, sp.nodes().size());
    const auto expected = std::vector<SpacePartition::area_t>{
        SpacePartition::area_t{begin, height, begin, width},
        SpacePartition::area_t{min_height, height, min_width, width},
        SpacePartition::area_t{begin, min_height, begin, min_width}
    };
    std::vector<SpacePartition::area_t> actual(size);
    boost::range::transform(sp.nodes(), actual.begin(), [&sp](const auto& node) { return sp.area(node); });
    EXPECT_THAT(actual, ::testing::ContainerEq(expected));
}