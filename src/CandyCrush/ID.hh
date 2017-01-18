/******************************************************************
* Copyright (C) 2016 Jordi Serrano Berbel <jsberbel95@gmail.com> *
* This can not be copied, modified and/or distributed without    *
* express permission of the copyright owner.                     *
******************************************************************/

#pragma once
#include <iostream>


// Enum ID used to identify each Game Object in the Game
enum class ObjectID : const int {
	BGFrogger, 
	GridDebug,
	FrogIUp,
	FrogIDown,
	FrogILeft,
	FrogIRight,
	BotIUp,
	Log_1, 
	Log_2,
	Log_3, 
	Car_1,
	Car_2,
	Car_3,
	Car_4,
	Car_Lorry,
	ObjectiveDone
};
inline std::ostream &operator<<(std::ostream &os, const ObjectID &id) { return os << int(id); };
inline std::istream &operator>>(std::istream &is, ObjectID &id) { return is >> reinterpret_cast<int&>(id); };



// Enum ID used to identify each Game Object in the Game
enum class BehaviorID : int {
	SAFE,
	RIP
};

// Enum ID used to store each true type font for text rendering
enum class FontID : int { 
	ARIAL,
	FACTORY, 
	MAX 
};

// Enum ID used to store each true type of main menu state
enum class MenuSceneState : int {
	DEFAULT,
	DIFFICULTY,
	CREDITS,
	RANKING, 
	COUNT
};


// Enum ID used to store each true type of game state
enum GameSceneState : int {
	NAME,
	PLAYING,
	PAUSED,
	GAMEOVER,
	COUNT
};

// Enum ID used to store each true type of main menu state
enum class Difficulty : int {
	EASY,
	MEDIUM,
	HARD
};

std::pair<int, int> typedef Coord;


#define RED {255, 0, 0}
#define GREEN {0, 255, 0}
#define BLUE {0, 0, 255}
#define WHITE {255, 255, 255}
#define BLACK {0, 0, 0}
#define YELLOW {255, 255, 0}
#define ORANGE {255, 127, 0}


#define GRID_WIDTH 14
#define GRID_HEIGHT 12
#define CELL_WIDTH 30
#define CELL_HEIGHT 35
#define GRID_Y_DISPLACEMENT 100
