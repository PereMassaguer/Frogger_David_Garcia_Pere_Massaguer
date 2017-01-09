#pragma once
#include "ID.hh"
#include "Sprite.hh"
#include "TimeManager.hh"

class Element
{
	float acumulateX;

protected:
	Sprite sprite;
	int size;
	int spawningRow;
	int direction;
	BehaviorID behavior;

public:
	Element();
	Element(int row);
	~Element();

	void Update();
	void Draw();
	virtual bool ExitedMap() = 0;
	std::vector<std::pair<Coord, BehaviorID>> CoordBehavior();
};