/******************************************************************
* Copyright (C) 2016 Jordi Serrano Berbel <jsberbel95@gmail.com> *
* This can not be copied, modified and/or distributed without    *
* express permission of the copyright owner.                     *
******************************************************************/

#pragma once
#include "Element.hh"
#include "Sprite.hh"
#include "InputManager.hh"





class Grid {
	//inline ObjectID &CandyID(int i, int j) const { return cellData[i][j].candy.objectID; }
	//inline Transform &CandyTransform(int i, int j) const { return cellData[i][j].candy.transform; }

	struct Cell { BehaviorID behaviorId; };
	Cell **cellData;

	int timer;
public:
	Grid();
	~Grid();

	void SetBehavior(std::vector<std::pair<Coord, BehaviorID>> &behaviors);
	void SetBehavior(Coord &coord, BehaviorID &behavior);
	BehaviorID GetBehavior(Coord &pos);
	void ResetGrid();


	void DebugGrid(int milliseconds);
};