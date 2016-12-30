#include "Car.hh"



Car::Car()
{
}

Car::Car(int i) : Element(i) {
	behavior = BehaviorID::RIP;
	direction = spawningRow % 2 != 0 ? 1 : -1;

	switch (spawningRow) {
	case 6:
		sprite.objectID = ObjectID::Car_Lorry;
		size = 2;
		break;
	case 7:
		sprite.objectID = ObjectID::Car_1;
		size = 1;
		break;
	case 8:
		sprite.objectID = ObjectID::Car_2;
		size = 1;
		break;
	case 9:
		sprite.objectID = ObjectID::Car_3;
		size = 1;
		break;
	case 10:
		sprite.objectID = ObjectID::Car_4;
		size = 1;
		break;
	default:
		system("pause");
		break;
	}
	sprite.transform = Transform(spawningRow % 2 != 0 ? 0 - size * CELL_WIDTH : W.GetWidth() - 1,
		GRID_Y_DISPLACEMENT + spawningRow * CELL_HEIGHT - 2, CELL_WIDTH * size, CELL_HEIGHT);
}

Car::~Car()
{
}

bool Car::ExitedMap()
{
	if (spawningRow % 2 != 0 && sprite.transform.x > W.GetWidth())
		return true;
	else if (spawningRow % 2 == 0 && sprite.transform.x < -CELL_WIDTH * size)
		return true;
	else
		return false;
}
