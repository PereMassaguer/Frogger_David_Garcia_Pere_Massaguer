#pragma once
#include "TimeManager.hh"
#include "Sprite.hh"

class Insect
{
	int appearTime;
	int lowestDuration;
	int variableDurationRange;
	int changeStateTime;

	bool isShown;
	int objectiveId;
	Sprite sprite;
	bool objectivesArrived[5];
	int xPositions[5];
public:
	Insect();
	~Insect();

	void Update();
	bool CheckGrabbed(int objectiveCounter);
	void Draw();
	void Reset();
};

