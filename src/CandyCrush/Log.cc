#include "Log.hh"



Log::Log()
{
}

Log::Log(int row) : Element(row)
{
	behavior = BehaviorID::SAFE;
	direction = spawningRow % 2 == 0 ? 1 : -1;
	timeToDestroy = 25000;

	int lenght = rand() % 3;
	switch (lenght) {
	case 0:
		sprite.objectID = ObjectID::Log_1;
		size = 3;
		break;
	case 1:
		sprite.objectID = ObjectID::Log_2;
		size = 4;
		break;
	case 2:
		sprite.objectID = ObjectID::Log_3;
		size = 6;
		break;
	}
	sprite.transform = Transform(spawningRow % 2 == 0 ? 0 - size * CELL_WIDTH : W.GetWidth() - 1,
		GRID_Y_DISPLACEMENT + spawningRow * CELL_HEIGHT + 5, CELL_WIDTH * size, 21);
}


Log::~Log()
{
}


bool Log::ExitedMap()
{
	timeToDestroy -= TM.GetDeltaTime();
	return timeToDestroy < 0 ? true : false;
}