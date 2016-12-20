#pragma once
#include "ID.hh"
#include "Sprite.hh"
#include "Transform.hh"


class Player
{
	Sprite sprite;
	int elementSize;
	float displacement;


public:
	Player();
	~Player();
	void Calculatetransform();

	void MoveUp();
	void MoveDown();
	void MoveLeft();
	void MoveRight();
	void CenterToClosestCell();
	void Draw();
};

