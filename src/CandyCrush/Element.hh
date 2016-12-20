#pragma once
#include "ID.hh"
#include "Sprite.hh"
#include "TimeManager.hh"

class Element
{
	int direction;
	Sprite sprite;
	float acumulateX;

public:
	Element();
	Element(int row, int lenght);
	~Element();

	void Update();
	void Draw();
	bool ExitedMap();
};

