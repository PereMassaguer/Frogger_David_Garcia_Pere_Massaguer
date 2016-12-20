#pragma once
#include "ID.hh"
#include "Sprite.hh"
#include "TimeManager.hh"

class Element
{
	int direction;
	Sprite sprite;
	BehaviorID behavior;
	int size;
	int spawningRow;
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
};

