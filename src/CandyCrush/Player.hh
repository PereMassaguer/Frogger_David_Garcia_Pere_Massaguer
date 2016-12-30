#pragma once
#include "ID.hh"
#include "Sprite.hh"
#include "TimeManager.hh"
#include "Transform.hh"


class Player
{
	Sprite sprite;
	float displacement;
	float acumulateX;

public:
	Player();
	~Player();

	void MoveUp();
	void MoveDown();
	void MoveLeft();
	void MoveRight();
	void Draw();
	void Update();
	void Reset();
	Coord GetGridCoords();
	Coord GetTransformCoords();

private:
	void ResetTransform();
	void CenterToClosestCell();
};