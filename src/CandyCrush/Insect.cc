#include "Insect.hh"



Insect::Insect()
{
	appearTime = TM.GetCurTime();
	lowestDuration = 15000;
	variableDurationRange = 5000;
	changeStateTime = lowestDuration + rand() % 5000;
	isShown = false;
	sprite.transform = Transform(0, GRID_Y_DISPLACEMENT - CELL_HEIGHT, CELL_WIDTH, CELL_HEIGHT);
	sprite.objectID = ObjectID::FrogIUp;
	xPositions[0] = 38 - CELL_WIDTH / 2;
	xPositions[1] = 122 - CELL_WIDTH / 2;
	xPositions[2] = 211 - CELL_WIDTH / 2;
	xPositions[3] = 298 - CELL_WIDTH / 2;
	xPositions[4] = 382 - CELL_WIDTH / 2;
}


Insect::~Insect()
{
}

void Insect::Update()
{
	if (TM.GetCurTime() > appearTime + changeStateTime) {
		isShown = !isShown;
		appearTime = TM.GetCurTime();
		changeStateTime = lowestDuration + rand() % 5000;
		objectiveId = rand() % 5;
		if (objectivesArrived[objectiveId]) {
			for (int i = 0; i < 5; i++) {
				if (!objectivesArrived[i]) {
					objectiveId = i;
					break;
				}
			}
		}
		sprite.transform.x = xPositions[objectiveId];
	}
}

bool Insect::CheckGrabbed(int objectiveCounter)
{
	objectivesArrived[objectiveCounter] = true;
	return (objectiveCounter) == objectiveId && isShown;
}

void Insect::Draw()
{
	if(isShown) sprite.Draw();
}

void Insect::Reset()
{
	appearTime = TM.GetCurTime();
	changeStateTime = lowestDuration + rand() % 5000;
	isShown = false;
}
