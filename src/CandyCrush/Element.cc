#include "Element.hh"



Element::Element()
{
}

Element::Element(int row, int lenght)
{
	sprite.objectID = ObjectID::FrogIUp;

	sprite.transform = Transform(row % 2 == 1 ? 0 : W.GetWidth(), GRID_Y_DISPLACEMENT + row * CELL_HEIGHT, CELL_WIDTH, CELL_HEIGHT);
	direction = row % 2 == 1 ? 1 : -1;
	acumulateX = 0.0f;
	spawningRow = row;
	behavior = BehaviorID::SAFE;

	switch (lenght) {
		case 0:
			sprite.objectID = ObjectID::Log_1;
			size = 3;
		case 1:
			sprite.objectID = ObjectID::Log_2;
			size = 4;
		case 2:
			sprite.objectID = ObjectID::Log_3;
			size = 6;
	}
	sprite.transform = Transform(row % 2 == 1 ? 0 : W.GetWidth(), GRID_Y_DISPLACEMENT + row * CELL_HEIGHT + 5, CELL_WIDTH * size, 21);
}

Element::Element(int row)
{
	sprite.objectID = ObjectID::FrogIUp;

	sprite.transform = Transform(row % 2 == 1 ? 0 : W.GetWidth(), GRID_Y_DISPLACEMENT + row * CELL_HEIGHT, CELL_WIDTH, CELL_HEIGHT);
	direction = row % 2 == 1 ? 1 : -1;
	acumulateX = 0.0f;
	size = 1;
	spawningRow = row;
	behavior = BehaviorID::SAFE;
}


Element::~Element()
{
	std::cout << "bye" << std::endl;
}

void Element::Update()
{
	acumulateX += TM.GetDeltaTime() / 20.0f;
	if (acumulateX >= 1.0f) {
		sprite.transform.x += (int)acumulateX;
		acumulateX -= (int)acumulateX;
	}
}

void Element::Draw()
{
	sprite.Draw();
}

bool Element::ExitedMap()
{
	return (sprite.transform.x < 0 || sprite.transform.x > W.GetWidth()) ? true : false;
}

std::vector<std::pair<Coord, BehaviorID>> Element::CoordBehavior() 
{
	std::vector<std::pair<Coord, BehaviorID>> ret;

	Coord aux;
	aux.first = sprite.transform.x / CELL_WIDTH;
	aux.second = (sprite.transform.y - GRID_Y_DISPLACEMENT) / CELL_HEIGHT;


	ret.push_back(std::make_pair(aux, behavior));
	for (int i = 0; i < size; i++) {
		aux.first += direction;
		if (aux.first == GRID_WIDTH || aux.first == -1) break;
		ret.push_back(std::make_pair(aux, behavior));
	}

	return ret;
}


std::vector<std::pair<Coord, BehaviorID>> Element::CoordBehavior(std::vector<std::pair<Coord, BehaviorID>> &ret)
{
	Coord aux;
	aux.first = sprite.transform.x / CELL_WIDTH;
	aux.second = (sprite.transform.y - GRID_Y_DISPLACEMENT) / CELL_HEIGHT;


	ret.push_back(std::make_pair(aux, behavior));
	for (int i = 0; i < size; i++) {
		aux.first += direction;
		if (aux.first == GRID_WIDTH || aux.first == -1) break;
		ret.push_back(std::make_pair(aux, behavior));
	}

	return ret;
}