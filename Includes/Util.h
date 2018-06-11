//-------------------------------------------------------------------------------------
// Util.h
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Written by Dale Green. Copyright (c) Packt Publishing. All rights reserved.
//-------------------------------------------------------------------------------------
#ifndef UTIL_H
#define UTIL_H

// Game states.
enum class GAME_STATE {
	MAIN_MENU,
	PLAYING,
	GAME_OVER
};

// Spawnable items.
enum class ITEM {
	GEM,
	GOLD,
	HEART,
	POTION,
	KEY,
	COUNT
};

// Enemy types.
enum class ENEMY {
	SLIME,
	HUMANOID,
	COUNT
};

// Animation states.
enum class ANIMATION_STATE {
	WALK_UP,
	WALK_DOWN,
	WALK_RIGHT,
	WALK_LEFT,
	IDLE_UP,
	IDLE_DOWN,
	IDLE_RIGHT,
	IDLE_LEFT,
	COUNT
};

// Tiles.
enum class TILE {
	WALL_SINGLE 		= 0,
	WALL_TOP_END		= 1,
	WALL_SIDE_RIGHT_END	= 2,
	WALL_BOTTOM_LEFT	= 3,
	WALL_BOTTOM_END		= 4,
	WALL_SIDE			= 5,
	WALL_TOP_LEFT		= 6,
	WALL_SIDE_LEFT_T	= 7,
	WALL_SIDE_LEFT_END	= 8,
	WALL_BOTTOM_RIGHT	= 9,
	WALL_TOP			=10,
	WALL_BOTTOM_T		=11,
	WALL_TOP_RIGHT		=12,
	WALL_SIDE_RIGHT_T	=13,
	WALL_TOP_T			=14,
	WALL_INTERSECTION	=15,
	WALL_DOOR_LOCKED	=16,
	WALL_DOOR_UNLOCKED	=17,
	WALL_ENTRANCE		=18,
	FLOOR				=19,
	FLOOR_ALT			=20,
	EMPTY				=21,
	COUNT				=22
};

// Game views.
enum class VIEW {
	MAIN,
	UI,
	COUNT
};
#endif