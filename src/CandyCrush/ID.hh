/******************************************************************
* Copyright (C) 2016 Jordi Serrano Berbel <jsberbel95@gmail.com> *
* This can not be copied, modified and/or distributed without    *
* express permission of the copyright owner.                     *
******************************************************************/

#pragma once
#include <iostream>

// Enum ID used to identify each Game Object in the Game
enum class ObjectID : const int {
	BGFrogger
};
inline std::ostream &operator<<(std::ostream &os, const ObjectID &id) { return os << int(id); };
inline std::istream &operator>>(std::istream &is, ObjectID &id) { return is >> reinterpret_cast<int&>(id); };

// Enum ID used to store each true type font for text rendering
enum class FontID : int { 
	ARIAL,
	FACTORY, 
	MAX 
};

// Enum ID used to store each true type font for text rendering
enum class ButtonID : int {
	PLAY,
	EXIT, 
	EASY, 
	MEDIUM,
	HARD
};

// Enum ID used to store each true type of main menu state
enum class MenuSceneState : int {
	DEFAULT,
	DIFFICULTY,
	RANKING
};

#define RED {255, 0, 0}
#define GREEN {0, 255, 0}
#define BLUE {0, 0, 255}
#define WHITE {255, 255, 255}
#define BLACK {0, 0, 0}
#define YELLOW {255, 255, 0}
#define ORANGE {255, 127, 0}
