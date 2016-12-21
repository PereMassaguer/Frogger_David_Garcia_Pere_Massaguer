#pragma once
#include "ID.hh"
#include "Sprite.hh"
#include "TimeManager.hh"

class Element
{
	Sprite sprite;
	BehaviorID behavior;
	int size;
	int spawningRow;
	int direction;
	float acumulateX;

public:
	Element();
	Element(int row, int lenght);
	Element(int row);
	~Element();

	void Update();
	void Draw();
	bool ExitedMap();
	std::vector<std::pair<Coord, BehaviorID>> CoordBehavior();
	std::vector<std::pair<Coord, BehaviorID>> CoordBehavior(std::vector<std::pair<Coord, BehaviorID>>& ret);
};

