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

struct RngMock
{
    MOCK_METHOD2(generate, int(int, int));
    auto getFunction()
    {
        return [this](int min, int max) { return generate(min, max); };
    }
};

struct TestSpatialLevelGenerator : public testing::Test
{
    TestSpatialLevelGenerator()
    : sut(rngMock.getFunction())
    {}

    RngMock rngMock;
    SpacePartition sut;
};

TEST_F(TestSpatialLevelGenerator, doesNotAcceptMinDimensionGreaterThenHalfOfThatDimension)
{
    const auto min_height = 2;
    const auto min_width = 2;
    const auto height = 3;
    const auto width = 3;
    ASSERT_FALSE(sut.divide(min_width, min_height, width, height));
}

TEST_F(TestSpatialLevelGenerator, twoRoomsHorizontally)
{
    EXPECT_CALL(rngMock, generate(0, 1)).WillRepeatedly(testing::Return(0));
    EXPECT_CALL(rngMock, generate(10 /*min*/, 10 /*max*/)).WillRepeatedly(testing::Return(10));
    
    const auto min_height = 10;
    const auto min_width = 10;
    const auto height = 20;
    const auto width = 20;

    ASSERT_TRUE(sut.divide(min_width, min_height, width, height));

    EXPECT_THAT(sut.rooms(), ::testing::UnorderedElementsAre(
                            SpacePartition::area_t{0, height, 11, width},
                            SpacePartition::area_t{0, height, 0, 10}));
}

TEST_F(TestSpatialLevelGenerator, twoRoomsVertically)
{
    EXPECT_CALL(rngMock, generate(0, 1)).WillRepeatedly(testing::Return(1));
    EXPECT_CALL(rngMock, generate(10 /*min*/, 10 /*max*/)).WillRepeatedly(testing::Return(10));
    
    const auto min_height = 10;
    const auto min_width = 10;
    const auto height = 20;
    const auto width = 20;

    ASSERT_TRUE(sut.divide(min_width, min_height, width, height));

    EXPECT_THAT(sut.rooms(), ::testing::UnorderedElementsAre(
                            SpacePartition::area_t{11, height, 0, width},
                            SpacePartition::area_t{0, 10, 0, width}));
}

TEST_F(TestSpatialLevelGenerator, threeRoomsHorizontally)
{
    EXPECT_CALL(rngMock, generate(testing::_ /*min*/, testing::_ /*max*/)).WillRepeatedly(testing::Invoke(
        [](int min, int max) { return (min == 0 and max == 1) ? 0 : 10; }));

    const auto min_height = 10;
    const auto min_width = 10;
    const auto height = 40;
    const auto width = 40;

    ASSERT_TRUE(sut.divide(min_width, min_height, width, height));

    EXPECT_THAT(sut.rooms(), ::testing::UnorderedElementsAre(
                            SpacePartition::area_t{0, height, 0, 10},
                            SpacePartition::area_t{0, height, 11, 21},
                            SpacePartition::area_t{0, height, 22, width}));
}