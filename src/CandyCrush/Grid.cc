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

	timer = TM.GetCurTime();
	ResetGrid();	
}

Grid::~Grid() {
	for (int i = 0; i < GRID_HEIGHT; ++i) delete[] cellData[i];
	delete[] cellData;
}



void Grid::SetBehavior(std::vector<std::pair<Coord, BehaviorID>> &behaviors)
{
	for (int i = 0; i < behaviors.size(); i++)
		cellData[behaviors[i].first.second][behaviors[i].first.first].behaviorId = behaviors[i].second;
}


void Grid::SetBehavior(std::pair<Coord, BehaviorID> &data)
{
	cellData[data.first.second][data.first.first].behaviorId = data.second;
}


BehaviorID Grid::GetBehavior(Coord &pos)
{
	return cellData[pos.second][pos.first].behaviorId;
}


void Grid::ResetGrid()
{
	for (int i = 0; i < GRID_HEIGHT; ++i) {
		for (int j = 0; j < GRID_WIDTH; ++j)
			(i >= 5) ? cellData[i][j].behaviorId = BehaviorID::SAFE : cellData[i][j].behaviorId = BehaviorID::RIP;
	}
}


void Grid::DebugGrid(int milliseconds) {
	if (TM.GetCurTime() - timer > milliseconds) {
		timer = TM.GetCurTime();
		DebugGrid();
	}
}


void Grid::DebugGrid() {
	system("cls");
	for (int i = 0; i < 5; ++i) {
		for (int j = 0; j < GRID_WIDTH; ++j) {
			if (cellData[i][j].behaviorId != BehaviorID::SAFE) std::cout << "- ";
			else { std::cout << (int)cellData[i][j].behaviorId << " "; }
		}
		std::cout << std::endl;
	}

	for (int j = 0; j < GRID_WIDTH; ++j) std::cout << "||";
	std::cout << std::endl;

	for (int i = 6; i < GRID_HEIGHT - 1; ++i) {
		for (int j = 0; j < GRID_WIDTH; ++j) {
			if (cellData[i][j].behaviorId == BehaviorID::SAFE) std::cout << "- ";
			else { std::cout << (int)cellData[i][j].behaviorId << " "; }
		}
		std::cout << std::endl;
	}

	for (int j = 0; j < GRID_WIDTH; ++j) std::cout << "||";
	std::cout << std::endl;
}