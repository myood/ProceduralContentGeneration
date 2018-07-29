#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <iostream>

#include "SpacePartition.h"
#include "SpatialLevelGenerator.h"
#include "Util.h"

struct GetRelativeProximityEqNone : testing::TestWithParam<SpacePartition::area_t> {};

TEST_P(GetRelativeProximityEqNone, shouldReturn)
{
    const auto A = SpacePartition::area_t{ 5, 5, 7, 7};
    ASSERT_EQ(RelativeProximity::None, getRelativeProximityType(A, GetParam()));
}

    /*
      0 1 2 3 4 5 6 7 8 9 A B
    0   
    1   O O O   O O O   O O O
    2   O 1 O   O 2 O   O 3 O
    3   O O O   O O O   O O O
    4
    5   O O O   O O O   O O O
    6   O 4 O   O A O   O 5 O
    7   O O O   O O O   O O O
    8 
    9   O O O   O O O   O O O
    A   O 6 O   O 7 O   O 8 O
    B   O O O   O O O   O O O
    */
INSTANTIATE_TEST_CASE_P(SeparatedRooms, GetRelativeProximityEqNone, ::testing::Values(
    SpacePartition::area_t{ 1, 1, 3, 3 },   //1
    SpacePartition::area_t{ 1, 5, 3, 7 },   //2
    SpacePartition::area_t{ 1, 9, 3, 11 },  //3
    SpacePartition::area_t{ 5, 1, 7, 3 },   //4
    SpacePartition::area_t{ 5, 9, 7, 11 },  //5
    SpacePartition::area_t{ 9, 1, 11, 3 },  //6
    SpacePartition::area_t{ 9, 5, 11, 7 },  //7
    SpacePartition::area_t{ 9, 9, 11, 11 }  //8
));

    /*
      0 1 2 3 4 5 6 7 8 9 A
    0
    1   
    2     O O O O O O O O O
    3     O 1 O O 2 O O 3 O
    4     O O O O O O O O O
    5     O O O O O O O O O
    6     O 4 O O A O O 5 O
    7     O O O O O O O O O
    8     O O O O O O O O O
    9     O 6 O O 7 O O 8 O
    A     O O O O O O O O O
    */
INSTANTIATE_TEST_CASE_P(StickyRooms, GetRelativeProximityEqNone, ::testing::Values(
    SpacePartition::area_t{ 2, 2, 4, 4 },   //1
    SpacePartition::area_t{ 2, 5, 4, 7 },   //2
    SpacePartition::area_t{ 2, 8, 4, 11 },  //3
    SpacePartition::area_t{ 5, 2, 8, 4 },   //4
    SpacePartition::area_t{ 5, 8, 8, 11 },  //5
    SpacePartition::area_t{ 8, 2, 11, 4 },  //6
    SpacePartition::area_t{ 8, 5, 11, 8 },  //7
    SpacePartition::area_t{ 8, 8, 11, 11 }  //8
));

    /*
      0 1 2 3 4 5 6 7 8 9 A
    0
    1       O O O   O O O
    2       O 1 O   O 2 O
    3 O O O O O O   O O O
    4 O 3 O
    5 O O O     O O O
    6           O A O
    7 O O O     O O O
    8 O 4 O
    9 O O O
    A
    */
INSTANTIATE_TEST_CASE_P(CornerCases, GetRelativeProximityEqNone, ::testing::Values(
    SpacePartition::area_t{ 1, 3, 3, 5 },  //1
    SpacePartition::area_t{ 1, 7, 3, 9 },  //2
    SpacePartition::area_t{ 3, 0, 5, 2 },  //3
    SpacePartition::area_t{ 7, 0, 9, 2 }   //4
));


    /*
      0 1 2 3 4 5 6 7 8 9
    0
    1
    2
    3       O O O   O O O
    4       O 1 O   O 2 O
    5       O O O O O O O
    6           O A O    
    7       O O O O O O O
    8       O 3 O   O 4 O
    9       O O O   O O O
    */
INSTANTIATE_TEST_CASE_P(DiagonalIsNotNeighbour, GetRelativeProximityEqNone, ::testing::Values(
    SpacePartition::area_t{ 3, 3, 5, 5 },  //1
    SpacePartition::area_t{ 3, 7, 5, 9 },  //2
    SpacePartition::area_t{ 7, 3, 9, 5 },  //3
    SpacePartition::area_t{ 7, 7, 9, 9 }   //4
));

using AreaAndRelativeProximity = std::pair<SpacePartition::area_t, RelativeProximity>;
struct GetRelativeProximityOtherThenNone : testing::TestWithParam<AreaAndRelativeProximity>{};

TEST_P(GetRelativeProximityOtherThenNone, shouldReturn)
{
    const auto A = SpacePartition::area_t{ 3, 3, 5, 5};
    const auto B = GetParam().first;
    const auto relativeProximity = GetParam().second;
    ASSERT_EQ(relativeProximity, getRelativeProximityType(A, B));
}

    /*
      0 1 2 3 4 5 6 7
    0
    1       O O O    
    2       O 1 O    
    3   O O O O O O O
    4   O 2 O A O 3 O
    5   O O O O O O O
    6       O 4 O  
    7       O O O    
    */
INSTANTIATE_TEST_CASE_P(EqualSizeNeighbours, GetRelativeProximityOtherThenNone, ::testing::Values(
    //1
    std::make_pair(SpacePartition::area_t{ 1, 3, 3, 5 }, RelativeProximity::A_UNDER_B),
    //2
    std::make_pair(SpacePartition::area_t{ 3, 1, 5, 3 }, RelativeProximity::A_on_the_RIGHT_of_B),
    //3
    std::make_pair(SpacePartition::area_t{ 3, 5, 5, 7 }, RelativeProximity::A_on_the_LEFT_of_B),
    //4
    std::make_pair(SpacePartition::area_t{ 5, 3, 7, 5 }, RelativeProximity::A_on_TOP_of_B)
));

    /*
      0 1 2 3 4 5 6 7
    0
    1   O O O O O O O
    2   O   O   2   O
    3   O   O O O O O
    4   O 1 O A O    
    5   O   O O O O O
    6   O   O   3   O
    7   O O O O O O O
    
      0 1 2 3 4 5 6 7
    0
    1     O O O O O O
    2     O  4  O   O
    3     O O O O   O
    4       O A O 5 O 
    5     O O O O   O
    6     O  6  O   O
    7     O O O O O O
    */
INSTANTIATE_TEST_CASE_P(DifferentSizeNeighbours, GetRelativeProximityOtherThenNone, ::testing::Values(
    //1
    std::make_pair(SpacePartition::area_t{ 1, 1, 7, 3 }, RelativeProximity::A_on_the_RIGHT_of_B),
    //2
    std::make_pair(SpacePartition::area_t{ 1, 3, 3, 7 }, RelativeProximity::A_UNDER_B),
    //3
    std::make_pair(SpacePartition::area_t{ 5, 3, 7, 7 }, RelativeProximity::A_on_TOP_of_B),
    //4
    std::make_pair(SpacePartition::area_t{ 1, 5, 7, 7 }, RelativeProximity::A_on_the_LEFT_of_B),
    //5
    std::make_pair(SpacePartition::area_t{ 1, 2, 3, 5 }, RelativeProximity::A_UNDER_B),
    //6
    std::make_pair(SpacePartition::area_t{ 5, 2, 7, 5 }, RelativeProximity::A_on_TOP_of_B)
));

struct GetRelativeProximityForBigRoomOtherThenNone : testing::TestWithParam<AreaAndRelativeProximity> {};


TEST_P(GetRelativeProximityForBigRoomOtherThenNone, shouldReturn)
{
    const auto A = SpacePartition::area_t{ 5, 5, 9, 10};
    const auto B = GetParam().first;
    const auto relativeProximity = GetParam().second;
    ASSERT_EQ(relativeProximity, getRelativeProximityType(A, B));
}

/*
    0 1 2 3 4 5 6 7 8 9 A B C D
  0
  1
  2
  3           O O O O O O 
  4       O O O  2  O 3 O O O
  5       O 1 O O O O O O 4 O
  6       O O O  X   X  O O O 
  7       O 5 O    X    O 6 O
  8       O O O  X   X  O O O
  9       O 7 O O O O O O 8 O
  A       O O O   O     O O O
  B           O 9 O 10  O
  C           O O O O O O 
*/
INSTANTIATE_TEST_CASE_P(DifferentSizeNeighbours, GetRelativeProximityForBigRoomOtherThenNone, ::testing::Values(
    //1
    std::make_pair(SpacePartition::area_t{ 4, 3, 6, 5 }, RelativeProximity::A_on_the_RIGHT_of_B),
    //2
    std::make_pair(SpacePartition::area_t{ 3, 5, 5, 8 }, RelativeProximity::A_UNDER_B),
    //3
    std::make_pair(SpacePartition::area_t{ 3, 8, 5, 10 }, RelativeProximity::A_UNDER_B),
    //4
    std::make_pair(SpacePartition::area_t{ 4, 10, 6, 12 }, RelativeProximity::A_on_the_LEFT_of_B),
    //5
    std::make_pair(SpacePartition::area_t{ 6, 3, 8, 5 }, RelativeProximity::A_on_the_RIGHT_of_B),
    //6
    std::make_pair(SpacePartition::area_t{ 6, 10, 8, 12 }, RelativeProximity::A_on_the_LEFT_of_B),
    //7
    std::make_pair(SpacePartition::area_t{ 8, 3, 10, 5 }, RelativeProximity::A_on_the_RIGHT_of_B),
    //8
    std::make_pair(SpacePartition::area_t{ 8, 10, 10, 12 }, RelativeProximity::A_on_the_LEFT_of_B),
    //9
    std::make_pair(SpacePartition::area_t{ 9, 5, 12, 7 }, RelativeProximity::A_on_TOP_of_B),
    //10
    std::make_pair(SpacePartition::area_t{ 9, 5, 12, 10 }, RelativeProximity::A_on_TOP_of_B)
));

TEST(NeighbourhoodTest, fourRooms)
{
    /*
      0 ... 5 ... 10
    0 OOOOOOOOOOOOOO
    . O  A  O  B   O
    5 OOOOOOOOOOOOOO
    . O  C  O  D   O
    10OOOOOOOOOOOOOO
    */
    const SpacePartition::Areas input{
        SpacePartition::area_t{ 0, 0, 5, 5 }, //A
        SpacePartition::area_t{ 0, 5, 5, 10}, //B
        SpacePartition::area_t{ 5, 0, 10, 5 }, //C
        SpacePartition::area_t{ 5, 5, 10, 10} //D
    };

    ASSERT_THAT(
        getNeighbourRooms(input),
        testing::UnorderedElementsAre(
            Neighbourhood{0u, 1u, RelativeProximity::A_on_the_LEFT_of_B},
            Neighbourhood{0u, 2u, RelativeProximity::A_on_TOP_of_B},
            Neighbourhood{1u, 3u, RelativeProximity::A_on_TOP_of_B},
            Neighbourhood{2u, 3u, RelativeProximity::A_on_the_LEFT_of_B}));
}

TEST(NeighbourhoodTest, sixRooms)
{
    /*
      0 ... 5 ... 10 ... 15
    0 OOOOOOOOOOOOOOOOOOOOO
    . O  A  O  B   O   E  O
    5 OOOOOOOOOOOOOOOOOOOOO
    . O  C  O  D   O   _  O
    10OOOOOOOOOOOOOOOOOOOOO
    */
    const SpacePartition::Areas input{
        SpacePartition::area_t{ 0, 0, 5, 5 }, //A
        SpacePartition::area_t{ 0, 5, 5, 10}, //B
        SpacePartition::area_t{ 5, 0, 10, 5 }, //C
        SpacePartition::area_t{ 5, 5, 10, 10}, //D
        SpacePartition::area_t{ 0, 10, 5, 15}, //E
        SpacePartition::area_t{ 5, 10, 10, 15} //F
    };

    ASSERT_THAT(
        getNeighbourRooms(input),
        testing::UnorderedElementsAre(
            Neighbourhood{0u, 1u, RelativeProximity::A_on_the_LEFT_of_B},
            Neighbourhood{0u, 2u, RelativeProximity::A_on_TOP_of_B},
            Neighbourhood{1u, 3u, RelativeProximity::A_on_TOP_of_B},
            Neighbourhood{2u, 3u, RelativeProximity::A_on_the_LEFT_of_B},
            Neighbourhood{1u, 4u, RelativeProximity::A_on_the_LEFT_of_B},
            Neighbourhood{3u, 5u, RelativeProximity::A_on_the_LEFT_of_B},
            Neighbourhood{4u, 5u, RelativeProximity::A_on_TOP_of_B}));
}

TEST(NeighbourhoodTest, firstWithLast)
{
    /*
      0 ... 5 ... 10 ... 15
    0 OOOOOOOOOOOOOOOOOOOOO
    . O 1st O last O   E  O
    5 OOOOOOOOOOOOOOOOOOOOO
    */
    const SpacePartition::Areas input{
        SpacePartition::area_t{ 0, 0, 5, 5 }, //1st
        SpacePartition::area_t{ 0, 10, 5, 15}, //E
        SpacePartition::area_t{ 0, 5, 5, 10}, //last
    };

    ASSERT_THAT(
        getNeighbourRooms(input),
        testing::UnorderedElementsAre(
            Neighbourhood{0u, 2u, RelativeProximity::A_on_the_LEFT_of_B},
            Neighbourhood{1u, 2u, RelativeProximity::A_on_the_LEFT_of_B}
        ));
}


TEST(TestSpatialLevelGenerator, rooms4squareSize10x10)
{
    const auto areas = SpacePartition::Areas{
        SpacePartition::area_t{ 0, 0, 4, 4 },
        SpacePartition::area_t{ 0, 4, 4, 8 },
        SpacePartition::area_t{ 4, 0, 8, 4 },
        SpacePartition::area_t{ 4, 4, 8, 8 },
    };
    const auto width = 9;
    const auto height = 9;
    const auto O = TILE::WALL_SINGLE;
    const auto _ = TILE::FLOOR;
    auto expectedGrid = std::vector<std::vector<TILE>>{
        { O, O, O, O, O, O, O, O, O },
        { O, _, _, _, O, _, _, _, O },
        { O, _, _, _, O, _, _, _, O },
        { O, _, _, _, O, _, _, _, O },
        { O, O, O, O, O, O, O, O, O },
        { O, _, _, _, O, _, _, _, O },
        { O, _, _, _, O, _, _, _, O },
        { O, _, _, _, O, _, _, _, O },
        { O, O, O, O, O, O, O, O, O }
    };

    ASSERT_EQ(Grid{expectedGrid}, Grid{createGrid(width, height, areas)});
}

TEST(TestSpatialLevelGenerator, rooms2squareSize10x5_OverlappingWalls)
{
    const auto areas = SpacePartition::Areas{
        SpacePartition::area_t{ 0, 0, 4, 4 },
        SpacePartition::area_t{ 0, 4, 4, 9 }
    };
    const auto width = 10;
    const auto height = 5;
    const auto O = TILE::WALL_SINGLE;
    const auto _ = TILE::FLOOR;
    auto expectedGrid = std::vector<std::vector<TILE>>{
        { O, O, O, O, O, O, O, O, O, O },
        { O, _, _, _, O, _, _, _, _, O },
        { O, _, _, _, O, _, _, _, _, O },
        { O, _, _, _, O, _, _, _, _, O },
        { O, O, O, O, O, O, O, O, O, O }
    };

    ASSERT_EQ(Grid{expectedGrid}, Grid{createGrid(width, height, areas)});
}

TEST(TestSpatialLevelGenerator, rooms2squareSize10x5_WallsInTheMiddle)
{
    const auto areas = SpacePartition::Areas{
        SpacePartition::area_t{ 1, 1, 3, 4 },
        SpacePartition::area_t{ 1, 4, 3, 8 }
    };
    const auto width = 10;
    const auto height = 5;
    const auto O = TILE::WALL_SINGLE;
    const auto _ = TILE::FLOOR;
    auto expectedGrid = std::vector<std::vector<TILE>>{
        { _, _, _, _, _, _, _, _, _, _ },
        { _, O, O, O, O, O, O, O, O, _ },
        { _, O, _, _, O, _, _, _, O, _ },
        { _, O, O, O, O, O, O, O, O, _ },
        { _, _, _, _, _, _, _, _, _, _ }
    };

    ASSERT_EQ(Grid{expectedGrid}, Grid{createGrid(width, height, areas)});
}

using DoorwayTestParam = std::tuple<SpacePartition::area_t, SpacePartition::area_t, RelativeProximity, uint, SpacePartition::area_t>;
struct GetDoorwayTest : testing::TestWithParam<DoorwayTestParam> {};

TEST_P(GetDoorwayTest, functional)
{
    const auto A = std::get<0>(GetParam());
    const auto B = std::get<1>(GetParam());
    const auto relativeProximity = std::get<2>(GetParam());
    const auto width = std::get<3>(GetParam());
    const auto doorway = std::get<4>(GetParam());

    ASSERT_EQ(doorway, getDoorway(A, B, relativeProximity, width));
}

constexpr uint width_one = 1;
INSTANTIATE_TEST_CASE_P(WidthOneEqualSizeRooms3x3, GetDoorwayTest, ::testing::Values(
    /* A B
      01234
    0 00000
    1 0 0 0
    2 00000    */
    std::make_tuple(SpacePartition::area_t{0, 0, 2, 2}, SpacePartition::area_t{0, 2, 2, 4}, RelativeProximity::A_on_the_LEFT_of_B, width_one, SpacePartition::area_t{1, 2, 1, 2}),

    /* B A
      01234
    0 00000
    1 0 0 0
    2 00000    */
    std::make_tuple(SpacePartition::area_t{0, 2, 2, 4}, SpacePartition::area_t{0, 0, 2, 4}, RelativeProximity::A_on_the_RIGHT_of_B, width_one, SpacePartition::area_t{1, 2, 1, 2}),

    /* A
       B
      012
    0 000
    1 0 0
    2 000
    3 0 0
    4 000 */
    std::make_tuple(SpacePartition::area_t{ 0, 0, 2, 2}, SpacePartition::area_t{2, 0, 4, 2}, RelativeProximity::A_on_TOP_ofB, width_one, SpacePartition::area_t{ 2, 1, 2, 1})
));

//room width not enugh for doorway
//width = 1, size = 5x3
//width = 1, different size, small in the center of big
//width = 1, different size, small in the corner of big
//width = 2, equal size odd
//width = 2, equal size even
//width = 2, different size, small in the center of big
//width = 2, different size, small in the corner of big