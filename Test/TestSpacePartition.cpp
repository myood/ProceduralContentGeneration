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

struct RandomNumberMock
{
    MOCK_METHOD2(generate, int(int, int));
    auto getFunction()
    {
        return [this](int min, int max) { return generate(min, max); };
    }
};

struct RandomBoolMock
{
    MOCK_METHOD0(generate, bool());
    auto getFunction()
    {
        return [this](){ return generate(); };
    }
};

struct TestSpacePartition : public testing::Test
{
    TestSpacePartition()
    : sut(randomNumberMock.getFunction(), randomBoolMock.getFunction())
    {
    }

    const int min_height = 2;
    const int min_width = 2;
    const int height = 3;
    const int width = 3;

    RandomNumberMock randomNumberMock;
    RandomBoolMock randomBoolMock;
    SpacePartition sut;
};

TEST_F(TestSpacePartition, doesNotAcceptMinDimensionGreaterThenHalfOfThatDimension)
{
    const auto max_rooms = 3;
    ASSERT_FALSE(sut.divide(max_rooms, min_width, min_height, width, height));
}

TEST_F(TestSpacePartition, dontSplitIfMaxRoomsZero)
{
    const auto max_rooms = 0;
    ASSERT_FALSE(sut.divide(max_rooms, min_width, min_height, width, height));
}

struct TestSpacePartition_Size20 : TestSpacePartition
{
    TestSpacePartition_Size20()
    {
        EXPECT_CALL(randomNumberMock, generate(10 /*min*/, 10 /*max*/)).WillRepeatedly(testing::Return(10));
        ON_CALL(randomBoolMock, generate()).WillByDefault(testing::Return(true));
    }
    
    const uint max_rooms = 2;
    const int min_height = 10;
    const int min_width = 10;
    const int height = 20;
    const int width = 20;
};

TEST_F(TestSpacePartition_Size20, doesNotAcceptZeroRoomsOrOneRoom)
{
    auto max_rooms = 0u;
    ASSERT_FALSE(sut.divide(max_rooms, min_width, min_height, width, height));
    max_rooms = 1u;
    ASSERT_FALSE(sut.divide(max_rooms, min_width, min_height, width, height));
}

TEST_F(TestSpacePartition_Size20, horizontally)
{
    EXPECT_CALL(randomNumberMock, generate(0, 1)).WillRepeatedly(testing::Return(0));

    ASSERT_TRUE(sut.divide(max_rooms, min_width, min_height, width, height));

    EXPECT_THAT(sut.rooms(), ::testing::UnorderedElementsAre(
                            SpacePartition::area_t{0, height, 10, width},
                            SpacePartition::area_t{0, height, 0, 10}));
}

TEST_F(TestSpacePartition_Size20, vertically)
{
    EXPECT_CALL(randomNumberMock, generate(0, 1)).WillRepeatedly(testing::Return(1));

    ASSERT_TRUE(sut.divide(max_rooms, min_width, min_height, width, height));

    EXPECT_THAT(sut.rooms(), ::testing::UnorderedElementsAre(
                            SpacePartition::area_t{10, height, 0, width},
                            SpacePartition::area_t{0, 10, 0, width}));
}

struct TestSpacePartition_Size40 : TestSpacePartition
{
    TestSpacePartition_Size40()
    {
        EXPECT_CALL(randomNumberMock, generate(testing::_ /*min*/, testing::_ /*max*/))
        .WillRepeatedly(testing::Invoke(
            [](int min, int max) { return (min == 0 and max == 1) ? 0 : 10; }));
        ON_CALL(randomBoolMock, generate()).WillByDefault(testing::Return(true));
    }
    
    const int min_height = 10;
    const int min_width = 10;
    const int height = 40;
    const int width = 40;
};

TEST_F(TestSpacePartition_Size40, horizontally)
{
    const uint max_rooms = 3;
    ASSERT_TRUE(sut.divide(max_rooms, min_width, min_height, width, height));

    EXPECT_THAT(sut.rooms(), ::testing::UnorderedElementsAre(
                            SpacePartition::area_t{0, height, 0, 10},
                            SpacePartition::area_t{0, height, 10, 20},
                            SpacePartition::area_t{0, height, 20, width}));
}

TEST_F(TestSpacePartition_Size40, stopSplitWhenMaxRoomsReached)
{
    const uint max_rooms = 2;
    ASSERT_TRUE(sut.divide(max_rooms, min_width, min_height, width, height));

    EXPECT_THAT(sut.rooms(), ::testing::UnorderedElementsAre(
                            SpacePartition::area_t{0, height, 0, 10},
                            SpacePartition::area_t{0, height, 10, width}));
}

TEST_F(TestSpacePartition_Size40, pseudoRandomSplit)
{
    int i = 0;
    EXPECT_CALL(randomBoolMock, generate())
    .Times(testing::AtLeast(1))
    .WillRepeatedly(testing::Invoke([&i](){ return static_cast<bool>(++i); }));

    const uint max_rooms = 9;
    ASSERT_TRUE(sut.divide(max_rooms, min_width, min_height, 100, 100));

    ASSERT_EQ(sut.rooms().size(), max_rooms);
    auto get_width = [](const auto& room) { return room.right - room.left; };
    auto get_height = [](const auto& room) { return room.bottom - room.top; };
    for (auto&& room : sut.rooms())
    {
        ASSERT_TRUE(get_width(room) >= min_width);
        ASSERT_TRUE(get_height(room) >= min_height);
    }
}

TEST_F(TestSpacePartition_Size40, pseudoRandomSplit_maxRooms100)
{
    int i = 0;
    EXPECT_CALL(randomBoolMock, generate())
    .Times(testing::AtLeast(1))
    .WillRepeatedly(testing::Invoke([&i](){ return static_cast<bool>(++i); }));

    const uint overshot_max_rooms = 100;
    ASSERT_TRUE(sut.divide(overshot_max_rooms, min_width, min_height, 100, 100));

    const uint realistic_max_rooms = 10;
    ASSERT_EQ(sut.rooms().size(), realistic_max_rooms);
    auto get_width = [](const auto& room) { return room.right - room.left; };
    auto get_height = [](const auto& room) { return room.bottom - room.top; };
    for (auto&& room : sut.rooms())
    {
        ASSERT_TRUE(get_width(room) >= min_width);
        ASSERT_TRUE(get_height(room) >= min_height);
    }
}