/******************************************************************
* Copyright (C) 2016 Jordi Serrano Berbel <jsberbel95@gmail.com> *
* This can not be copied, modified and/or distributed without    *
* express permission of the copyright owner.                     *
******************************************************************/

#include <ctime>
#include "Grid.hh"
#include "Window.hh"
#include "IOManager.hh"
#include "TimeManager.hh"



Grid::Grid() {
	cellData = new Cell*[GRID_HEIGHT];
	for (int i = 0; i < GRID_HEIGHT; ++i) cellData[i] = new Cell[GRID_WIDTH];
	srand(unsigned(time(nullptr)));


	int isHarmful[GRID_HEIGHT][GRID_WIDTH]{
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
	};

	for (int i = 0; i < GRID_HEIGHT; ++i) {
		for (int j = 0; j < GRID_WIDTH; ++j) {
			if(isHarmful[i][j] == 0) cellData[i][j].behaviorId = BehaviorID::SAFE;
			else { cellData[i][j].behaviorId = BehaviorID::RIP; }
		}
	}
}

Grid::~Grid() {
	for (int i = 0; i < GRID_HEIGHT; ++i) delete[] cellData[i];
	delete[] cellData;
}

void Grid::SetBehavior(Transform transform)
{

}


BehaviorID Grid::GetBehavior(Coord pos)
{
	return cellData[pos.second][pos.first].behaviorId;
}

void Grid::Update(int &score) {
	
}

void Grid::Draw() {/*
	for (int i = 0; i < m_rows; ++i) for (int j = 0; j < m_cols; ++j) cellData[i][j].Draw();
	for (int i = 0; i < m_rows; ++i) for (int j = 0; j < m_cols; ++j) if (CandyID(i, j) != ObjectID::CANDY_EMPTY) cellData[i][j].candy.Draw();
*/}