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

struct RbgMock
{
    MOCK_METHOD0(generate, bool());
    auto getFunction()
    {
        return [this](){ return generate(); };
    }
};

struct TestSpatialLevelGenerator : public testing::Test
{
    TestSpatialLevelGenerator()
    : sut(rngMock.getFunction(), rbgMock.getFunction())
    {}

    const int min_height = 2;
    const int min_width = 2;
    const int height = 3;
    const int width = 3;

    RngMock rngMock;
    RbgMock rbgMock;
    SpacePartition sut;
};

TEST_F(TestSpatialLevelGenerator, doesNotAcceptMinDimensionGreaterThenHalfOfThatDimension)
{
    const auto max_rooms = 100;
    ASSERT_FALSE(sut.divide(max_rooms, min_width, min_height, width, height));
}

TEST_F(TestSpatialLevelGenerator, dontSplitIfMaxRoomsZero)
{
    const auto max_rooms = 0;
    ASSERT_FALSE(sut.divide(max_rooms, min_width, min_height, width, height));
}

struct TestSpatialLevelGenerator_Size20 : TestSpatialLevelGenerator
{
    TestSpatialLevelGenerator_Size20()
    {
        EXPECT_CALL(rngMock, generate(10 /*min*/, 10 /*max*/)).WillRepeatedly(testing::Return(10));
        ON_CALL(rbgMock, generate()).WillByDefault(testing::Return(true));
    }
    
    const uint max_rooms = 100;
    const int min_height = 10;
    const int min_width = 10;
    const int height = 20;
    const int width = 20;
};

TEST_F(TestSpatialLevelGenerator_Size20, horizontally)
{
    EXPECT_CALL(rngMock, generate(0, 1)).WillRepeatedly(testing::Return(0));

    ASSERT_TRUE(sut.divide(max_rooms, min_width, min_height, width, height));

    EXPECT_THAT(sut.rooms(), ::testing::UnorderedElementsAre(
                            SpacePartition::area_t{0, height, 11, width},
                            SpacePartition::area_t{0, height, 0, 10}));
}

TEST_F(TestSpatialLevelGenerator_Size20, vertically)
{
    EXPECT_CALL(rngMock, generate(0, 1)).WillRepeatedly(testing::Return(1));

    ASSERT_TRUE(sut.divide(max_rooms, min_width, min_height, width, height));

    EXPECT_THAT(sut.rooms(), ::testing::UnorderedElementsAre(
                            SpacePartition::area_t{11, height, 0, width},
                            SpacePartition::area_t{0, 10, 0, width}));
}

struct TestSpatialLevelGenerator_Size40 : TestSpatialLevelGenerator
{
    TestSpatialLevelGenerator_Size40()
    {
        EXPECT_CALL(rngMock, generate(testing::_ /*min*/, testing::_ /*max*/))
        .WillRepeatedly(testing::Invoke(
            [](int min, int max) { return (min == 0 and max == 1) ? 0 : 10; }));
        ON_CALL(rbgMock, generate()).WillByDefault(testing::Return(true));
    }
    
    const int min_height = 10;
    const int min_width = 10;
    const int height = 40;
    const int width = 40;
};

TEST_F(TestSpatialLevelGenerator_Size40, horizontally)
{
    const uint max_rooms = 100;
    ASSERT_TRUE(sut.divide(max_rooms, min_width, min_height, width, height));

    EXPECT_THAT(sut.rooms(), ::testing::UnorderedElementsAre(
                            SpacePartition::area_t{0, height, 0, 10},
                            SpacePartition::area_t{0, height, 11, 21},
                            SpacePartition::area_t{0, height, 22, width}));
}

TEST_F(TestSpatialLevelGenerator_Size40, stopSplitWhenMaxRoomsReached)
{
    const uint max_rooms = 2;
    ASSERT_TRUE(sut.divide(max_rooms, min_width, min_height, width, height));

    EXPECT_THAT(sut.rooms(), ::testing::UnorderedElementsAre(
                            SpacePartition::area_t{0, height, 0, 10},
                            SpacePartition::area_t{0, height, 11, width}));
}