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
#include "SpacePartition.h"

using namespace testing;

struct TestSpatialLevelGenerator : public Test
{
};

struct RngMock
{
    MOCK_METHOD2(generate, int(int, int));
    auto getFunction()
    {
        return [this](int min, int max) { return generate(min, max); };
    }
};

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
    EXPECT_CALL(rngMock, generate(10 /*min*/, 10 /*max*/)).WillRepeatedly(testing::Return(10));
    SpacePartition sp{rngMock.getFunction()};
    const auto min_height = 10;
    const auto min_width = 10;
    const auto height = 20;
    const auto width = 20;
    const auto begin = 0;
    ASSERT_TRUE(sp.divide(min_width, min_height, width, height));
    const auto size = 3;
    ASSERT_EQ(size, sp.nodes().size());
    std::vector<SpacePartition::area_t> actual(size);
    boost::range::transform(sp.nodes(), actual.begin(), [&sp](const auto &node) { return sp.area(node); });
    EXPECT_THAT(actual, ::testing::UnorderedElementsAre(
                            SpacePartition::area_t{begin, height, begin, width},
                            SpacePartition::area_t{begin, height, min_width, width},
                            SpacePartition::area_t{begin, height, begin, min_width}));
}