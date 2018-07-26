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
    const auto X = SpacePartition::area_t{ 5, 5, 7, 7};
    ASSERT_FALSE(isNeighbour(X, GetParam()));
}

    /*
      0 1 2 3 4 5 6 7 8 9 A B
    0   
    1   W W W   W W W   W W W
    2   W A W   W B W   W C W
    3   W W W   W W W   W W W
    4
    5   W W W   W W W   W W W
    6   W D W   W X W   W E W
    7   W W W   W W W   W W W
    8 
    9   W W W   W W W   W W W
    A   W F W   W G W   W H W
    B   W W W   W W W   W W W
    */
INSTANTIATE_TEST_CASE_P(SeparatedRooms, IsNeighbourFalse, ::testing::Values(
    SpacePartition::area_t{ 1, 1, 3, 3 },   //A
    SpacePartition::area_t{ 1, 5, 3, 7 },   //B
    SpacePartition::area_t{ 1, 9, 3, 11 },  //C
    SpacePartition::area_t{ 5, 1, 7, 3 },   //D
    SpacePartition::area_t{ 5, 9, 7, 11 },  //E
    SpacePartition::area_t{ 9, 1, 11, 3 },  //F
    SpacePartition::area_t{ 9, 5, 11, 7 },  //G
    SpacePartition::area_t{ 9, 9, 11, 11 }  //H
));

    /*
      0 1 2 3 4 5 6 7 8 9 A
    0
    1   
    2     W W W W W W W W W
    3     W A W W B W W C W
    4     W W W W W W W W W
    5     W W W W W W W W W
    6     W D W W X W W E W
    7     W W W W W W W W W
    8     W W W W W W W W W
    9     W F W W G W W H W
    A     W W W W W W W W W
    */
INSTANTIATE_TEST_CASE_P(StickyRooms, IsNeighbourFalse, ::testing::Values(
    SpacePartition::area_t{ 2, 2, 4, 4 },   //A
    SpacePartition::area_t{ 2, 5, 4, 7 },   //B
    SpacePartition::area_t{ 2, 8, 4, 11 },  //C
    SpacePartition::area_t{ 5, 2, 8, 4 },   //D
    SpacePartition::area_t{ 5, 8, 8, 11 },  //E
    SpacePartition::area_t{ 8, 2, 11, 4 },  //F
    SpacePartition::area_t{ 8, 5, 11, 8 },  //G
    SpacePartition::area_t{ 8, 8, 11, 11 }  //H
));

    /*
      0 1 2 3 4 5 6 7 8 9 A
    0
    1       W W W   W W W
    2       W A W   W B W
    3 W W W W W W   W W W
    4 W C W
    5 W W W     W W W
    6           W X W
    7 W W W     W W W
    8 W D W
    9 W W W
    A
    */
INSTANTIATE_TEST_CASE_P(CornerCases, IsNeighbourFalse, ::testing::Values(
    SpacePartition::area_t{ 1, 3, 3, 5 },  //A
    SpacePartition::area_t{ 1, 7, 3, 9 },  //B
    SpacePartition::area_t{ 3, 0, 5, 2 },  //C
    SpacePartition::area_t{ 7, 0, 9, 2 }   //D
));


    /*
      0 1 2 3 4 5 6 7 8 9
    0
    1
    2
    3       W W W   W W W
    4       W A W   W C W
    5       W W W W W W W
    6           W X W    
    7       W W W W W W W
    8       W F W   W H W
    9       W W W   W W W
    */
INSTANTIATE_TEST_CASE_P(DiagonalIsNotNeighbour, IsNeighbourFalse, ::testing::Values(
    SpacePartition::area_t{ 3, 3, 5, 5 },  //A
    SpacePartition::area_t{ 3, 7, 5, 9 },  //C
    SpacePartition::area_t{ 7, 3, 9, 5 },  //F
    SpacePartition::area_t{ 7, 7, 9, 9 }   //H
));

struct IsNeighbourTrue : testing::TestWithParam<SpacePartition::area_t> {};

TEST_P(IsNeighbourTrue, shouldReturn)
{
    const auto X = SpacePartition::area_t{ 3, 3, 5, 5};
    ASSERT_TRUE(isNeighbour(X, GetParam()));
}

    /*
      0 1 2 3 4 5 6 7
    0
    1       W W W    
    2       W B W    
    3   W W W W W W W
    4   W D W X W E W
    5   W W W W W W W
    6       W G W  
    7       W W W    
    */
INSTANTIATE_TEST_CASE_P(EqualSizeNeighbours, IsNeighbourTrue, ::testing::Values(
    SpacePartition::area_t{ 1, 3, 3, 5 },  //B
    SpacePartition::area_t{ 3, 1, 5, 3 },  //D
    SpacePartition::area_t{ 3, 5, 5, 7 },  //E
    SpacePartition::area_t{ 5, 3, 7, 5 }   //G
));

    /*
      0 1 2 3 4 5 6 7
    0
    1   W W W W W W W
    2   W   W   B   W
    3   W   W W W W W
    4   W A W X W    
    5   W   W W W W W
    6   W   W   C   W
    7   W W W W W W W
    
      0 1 2 3 4 5 6 7
    0
    1     W W W W W W
    2     W  E  W   W
    3     W W W W   W
    4       W X W D W 
    5     W W W W   W
    6     W  F  W   W
    7     W W W W W W
    */
INSTANTIATE_TEST_CASE_P(DifferentSizeNeighbours, IsNeighbourTrue, ::testing::Values(
    SpacePartition::area_t{ 1, 1, 3, 7 },  //A
    SpacePartition::area_t{ 1, 3, 3, 7 },  //B
    SpacePartition::area_t{ 5, 3, 7, 7 },  //C
    SpacePartition::area_t{ 1, 5, 7, 7 },  //D
    SpacePartition::area_t{ 1, 2, 3, 5 },  //E
    SpacePartition::area_t{ 5, 2, 7, 5 }   //F
));

struct IsNeighbourForBigTrue : testing::TestWithParam<SpacePartition::area_t> {};


TEST_P(IsNeighbourForBigTrue, shouldReturn)
{
    const auto X = SpacePartition::area_t{ 5, 5, 9, 10};
    ASSERT_TRUE(isNeighbour(X, GetParam()));
}

/*
    0 1 2 3 4 5 6 7 8 9 A B C D
  0
  1
  2
  3           W W W W W W 
  4       W W W  B  W C W W W
  5       W A W W W W W W D W
  6       W W W  X   X  W W W 
  7       W E W    X    W F W
  8       W W W  X   X  W W W
  9       W G W W W W W W H W
  A       W W W   W     W W W
  B           W I W  J  W
  C           W W W W W W 
*/
INSTANTIATE_TEST_CASE_P(DifferentSizeNeighbours, IsNeighbourForBigTrue, ::testing::Values(
    SpacePartition::area_t{ 4, 3, 6, 5 },   //A
    SpacePartition::area_t{ 3, 5, 5, 8 },   //B
    SpacePartition::area_t{ 3, 8, 5, 10 },  //C
    SpacePartition::area_t{ 4, 10, 6, 12 }, //D
    SpacePartition::area_t{ 6, 3, 8, 5 },   //E
    SpacePartition::area_t{ 6, 10, 8, 12 }, //F
    SpacePartition::area_t{ 8, 3, 10, 5 },  //G
    SpacePartition::area_t{ 8, 10, 10, 12 },//H
    SpacePartition::area_t{ 9, 5, 12, 7 },  //I
    SpacePartition::area_t{ 9, 5, 12, 10 }  //J
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