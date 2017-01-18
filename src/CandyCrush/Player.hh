#pragma once
#include "ID.hh"
#include "Sprite.hh"
#include "TimeManager.hh"
#include "Transform.hh"


class Player
{
	Sprite sprite;
	float acumulateX;
	int startX;

public:
	Player();
	Player(int xPos);
	~Player();

	void MoveUp();
	void MoveDown();
	void MoveLeft();
	void MoveRight();
	void Draw();
	void Update(float velocity);
	void Reset();
	Coord GetGridCoords();
	Coord GetTransformCoords();

private:
	void ResetTransform();
	void CenterToClosestCell();
};