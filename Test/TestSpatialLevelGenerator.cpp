#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <iostream>

#include "SpacePartition.h"
#include "SpatialLevelGenerator.h"
#include "Util.h"

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
    const auto W = TILE::WALL_SINGLE;
    const auto F = TILE::FLOOR;
    auto expectedGrid = std::vector<std::vector<TILE>>{
        { W, W, W, W, W, W, W, W, W },
        { W, F, F, F, W, F, F, F, W },
        { W, F, F, F, W, F, F, F, W },
        { W, F, F, F, W, F, F, F, W },
        { W, W, W, W, W, W, W, W, W },
        { W, F, F, F, W, F, F, F, W },
        { W, F, F, F, W, F, F, F, W },
        { W, F, F, F, W, F, F, F, W },
        { W, W, W, W, W, W, W, W, W }
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
    const auto W = TILE::WALL_SINGLE;
    const auto F = TILE::FLOOR;
    auto expectedGrid = std::vector<std::vector<TILE>>{
        { W, W, W, W, W, W, W, W, W, W },
        { W, F, F, F, W, F, F, F, F, W },
        { W, F, F, F, W, F, F, F, F, W },
        { W, F, F, F, W, F, F, F, F, W },
        { W, W, W, W, W, W, W, W, W, W }
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
    const auto W = TILE::WALL_SINGLE;
    const auto F = TILE::FLOOR;
    auto expectedGrid = std::vector<std::vector<TILE>>{
        { F, F, F, F, F, F, F, F, F, F },
        { F, W, W, W, W, W, W, W, W, F },
        { F, W, F, F, W, F, F, F, W, F },
        { F, W, W, W, W, W, W, W, W, F },
        { F, F, F, F, F, F, F, F, F, F }
    };

    ASSERT_EQ(Grid{expectedGrid}, Grid{createGrid(width, height, areas)});
}

struct IsNeighbourFalse : testing::TestWithParam<SpacePartition::area_t> {};

TEST_P(IsNeighbourFalse, shouldReturn)
{
    const auto A = SpacePartition::area_t{ 5, 5, 7, 7};
    ASSERT_EQ(Neighbourhood::None, getNeighbourhoodType(A, GetParam()));
}

    /*
      0 1 2 3 4 5 6 7 8 9 A B
    0   
    1   W W W   W W W   W W W
    2   W 1 W   W 2 W   W 3 W
    3   W W W   W W W   W W W
    4
    5   W W W   W W W   W W W
    6   W 4 W   W A W   W 5 W
    7   W W W   W W W   W W W
    8 
    9   W W W   W W W   W W W
    A   W 6 W   W 7 W   W 8 W
    B   W W W   W W W   W W W
    */
INSTANTIATE_TEST_CASE_P(SeparatedRooms, IsNeighbourFalse, ::testing::Values(
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
    2     W W W W W W W W W
    3     W 1 W W 2 W W 3 W
    4     W W W W W W W W W
    5     W W W W W W W W W
    6     W 4 W W A W W 5 W
    7     W W W W W W W W W
    8     W W W W W W W W W
    9     W 6 W W 7 W W 8 W
    A     W W W W W W W W W
    */
INSTANTIATE_TEST_CASE_P(StickyRooms, IsNeighbourFalse, ::testing::Values(
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
    1       W W W   W W W
    2       W 1 W   W 2 W
    3 W W W W W W   W W W
    4 W 3 W
    5 W W W     W W W
    6           W A W
    7 W W W     W W W
    8 W 4 W
    9 W W W
    A
    */
INSTANTIATE_TEST_CASE_P(CornerCases, IsNeighbourFalse, ::testing::Values(
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
    3       W W W   W W W
    4       W 1 W   W 2 W
    5       W W W W W W W
    6           W A W    
    7       W W W W W W W
    8       W 3 W   W 4 W
    9       W W W   W W W
    */
INSTANTIATE_TEST_CASE_P(DiagonalIsNotNeighbour, IsNeighbourFalse, ::testing::Values(
    SpacePartition::area_t{ 3, 3, 5, 5 },  //1
    SpacePartition::area_t{ 3, 7, 5, 9 },  //2
    SpacePartition::area_t{ 7, 3, 9, 5 },  //3
    SpacePartition::area_t{ 7, 7, 9, 9 }   //4
));

using AreaAndNeighbourhood = std::pair<SpacePartition::area_t, Neighbourhood>;
struct IsNeighbourTrue : testing::TestWithParam<AreaAndNeighbourhood>{};

TEST_P(IsNeighbourTrue, shouldReturn)
{
    const auto A = SpacePartition::area_t{ 3, 3, 5, 5};
    const auto B = GetParam().first;
    const auto neighbourhood = GetParam().second;
    ASSERT_EQ(neighbourhood, getNeighbourhoodType(A, B));
}

    /*
      0 1 2 3 4 5 6 7
    0
    1       W W W    
    2       W 1 W    
    3   W W W W W W W
    4   W 2 W A W 3 W
    5   W W W W W W W
    6       W 4 W  
    7       W W W    
    */
INSTANTIATE_TEST_CASE_P(EqualSizeNeighbours, IsNeighbourTrue, ::testing::Values(
    //1
    std::make_pair(SpacePartition::area_t{ 1, 3, 3, 5 }, Neighbourhood::A_UNDER_B),
    //2
    std::make_pair(SpacePartition::area_t{ 3, 1, 5, 3 }, Neighbourhood::A_on_the_RIGHT_of_B),
    //3
    std::make_pair(SpacePartition::area_t{ 3, 5, 5, 7 }, Neighbourhood::A_on_the_LEFT_of_B),
    //4
    std::make_pair(SpacePartition::area_t{ 5, 3, 7, 5 }, Neighbourhood::A_on_TOP_of_B)
));

    /*
      0 1 2 3 4 5 6 7
    0
    1   W W W W W W W
    2   W   W   2   W
    3   W   W W W W W
    4   W 1 W A W    
    5   W   W W W W W
    6   W   W   3   W
    7   W W W W W W W
    
      0 1 2 3 4 5 6 7
    0
    1     W W W W W W
    2     W  4  W   W
    3     W W W W   W
    4       W A W 5 W 
    5     W W W W   W
    6     W  6  W   W
    7     W W W W W W
    */
INSTANTIATE_TEST_CASE_P(DifferentSizeNeighbours, IsNeighbourTrue, ::testing::Values(
    //1
    std::make_pair(SpacePartition::area_t{ 1, 1, 7, 3 }, Neighbourhood::A_on_the_RIGHT_of_B),
    //2
    std::make_pair(SpacePartition::area_t{ 1, 3, 3, 7 }, Neighbourhood::A_UNDER_B),
    //3
    std::make_pair(SpacePartition::area_t{ 5, 3, 7, 7 }, Neighbourhood::A_on_TOP_of_B),
    //4
    std::make_pair(SpacePartition::area_t{ 1, 5, 7, 7 }, Neighbourhood::A_on_the_LEFT_of_B),
    //5
    std::make_pair(SpacePartition::area_t{ 1, 2, 3, 5 }, Neighbourhood::A_UNDER_B),
    //6
    std::make_pair(SpacePartition::area_t{ 5, 2, 7, 5 }, Neighbourhood::A_on_TOP_of_B)
));

struct IsNeighbourForBigTrue : testing::TestWithParam<AreaAndNeighbourhood> {};


TEST_P(IsNeighbourForBigTrue, shouldReturn)
{
    const auto A = SpacePartition::area_t{ 5, 5, 9, 10};
    const auto B = GetParam().first;
    const auto neighbourhood = GetParam().second;
    ASSERT_EQ(neighbourhood, getNeighbourhoodType(A, B));
}

/*
    0 1 2 3 4 5 6 7 8 9 A B C D
  0
  1
  2
  3           W W W W W W 
  4       W W W  2  W 3 W W W
  5       W 1 W W W W W W 4 W
  6       W W W  X   X  W W W 
  7       W 5 W    X    W 6 W
  8       W W W  X   X  W W W
  9       W 7 W W W W W W 8 W
  A       W W W   W     W W W
  B           W 9 W 10  W
  C           W W W W W W 
*/
INSTANTIATE_TEST_CASE_P(DifferentSizeNeighbours, IsNeighbourForBigTrue, ::testing::Values(
    //1
    std::make_pair(SpacePartition::area_t{ 4, 3, 6, 5 }, Neighbourhood::A_on_the_RIGHT_of_B),
    //2
    std::make_pair(SpacePartition::area_t{ 3, 5, 5, 8 }, Neighbourhood::A_UNDER_B),
    //3
    std::make_pair(SpacePartition::area_t{ 3, 8, 5, 10 }, Neighbourhood::A_UNDER_B),
    //4
    std::make_pair(SpacePartition::area_t{ 4, 10, 6, 12 }, Neighbourhood::A_on_the_LEFT_of_B),
    //5
    std::make_pair(SpacePartition::area_t{ 6, 3, 8, 5 }, Neighbourhood::A_on_the_RIGHT_of_B),
    //6
    std::make_pair(SpacePartition::area_t{ 6, 10, 8, 12 }, Neighbourhood::A_on_the_LEFT_of_B),
    //7
    std::make_pair(SpacePartition::area_t{ 8, 3, 10, 5 }, Neighbourhood::A_on_the_RIGHT_of_B),
    //8
    std::make_pair(SpacePartition::area_t{ 8, 10, 10, 12 }, Neighbourhood::A_on_the_LEFT_of_B),
    //9
    std::make_pair(SpacePartition::area_t{ 9, 5, 12, 7 }, Neighbourhood::A_on_TOP_of_B),
    //10
    std::make_pair(SpacePartition::area_t{ 9, 5, 12, 10 }, Neighbourhood::A_on_TOP_of_B)
));

TEST(CreateConnections, fourRooms)
{
    /*
      0 ... 5 ... 10
    0 WWWWWWWWWWWWWW
    . W  A  W  B   W
    5 WWWWWWWWWWWWWW
    . W  C  W  D   W
    10WWWWWWWWWWWWWW
    */
    const SpacePartition::Areas input{
        SpacePartition::area_t{ 0, 0, 5, 5 }, //A
        SpacePartition::area_t{ 0, 5, 5, 10}, //B
        SpacePartition::area_t{ 5, 0, 10, 5 }, //C
        SpacePartition::area_t{ 5, 5, 10, 10} //D
    };

    ASSERT_THAT(
        createConnections(input),
        testing::UnorderedElementsAre(
            Connection{0u, 1u},
            Connection{0u, 2u},
            Connection{1u, 3u},
            Connection{2u, 3u}));
}

TEST(CreateConnections, sixRooms)
{
    /*
      0 ... 5 ... 10 ... 15
    0 WWWWWWWWWWWWWWWWWWWWW
    . W  A  W  B   W   E  W
    5 WWWWWWWWWWWWWWWWWWWWW
    . W  C  W  D   W   F  W
    10WWWWWWWWWWWWWWWWWWWWW
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
        createConnections(input),
        testing::UnorderedElementsAre(
            Connection{0u, 1u},
            Connection{0u, 2u},
            Connection{1u, 3u},
            Connection{2u, 3u},
            Connection{1u, 4u},
            Connection{3u, 5u},
            Connection{4u, 5u}));
}

TEST(CreateConnections, firstWithLast)
{
    /*
      0 ... 5 ... 10 ... 15
    0 WWWWWWWWWWWWWWWWWWWWW
    . W 1st W last W   E  W
    5 WWWWWWWWWWWWWWWWWWWWW
    */
    const SpacePartition::Areas input{
        SpacePartition::area_t{ 0, 0, 5, 5 }, //1st
        SpacePartition::area_t{ 0, 10, 5, 15}, //E
        SpacePartition::area_t{ 0, 5, 5, 10}, //last
    };

    ASSERT_THAT(
        createConnections(input),
        testing::UnorderedElementsAre(
            Connection{0, 2},
            Connection{1, 2}
        ));
}