#include "Element.hh"



Element::Element()
{
}

Element::Element(int row)
{
	acumulateX = 0.0f;
	spawningRow = row;
}


Element::~Element()
{

}

void Element::Update()
{
	
	acumulateX += TM.GetDeltaTime() / 20.0f;
	if (acumulateX >= 1.0f) {
		sprite.transform.x += direction * (int)acumulateX;
		acumulateX -= (int)acumulateX;
	}
}

void Element::Draw()
{
	sprite.Draw();
}


std::vector<std::pair<Coord, BehaviorID>> Element::CoordBehavior() 
{
	std::vector<std::pair<Coord, BehaviorID>> ret;

	Coord aux;
	aux.first = sprite.transform.x / CELL_WIDTH;
	aux.second = (sprite.transform.y - GRID_Y_DISPLACEMENT) / CELL_HEIGHT;
	if (aux.second >= 5) aux.second++;

	for (int i = 0; i <= size; i++) {
		if (aux.first < GRID_WIDTH && aux.first >= 0) 
			ret.push_back(std::make_pair(aux, behavior));
		aux.first++;
	}
	return ret;
}


//std::vector<std::pair<Coord, BehaviorID>> Element::CoordBehavior(std::vector<std::pair<Coord, BehaviorID>> &ret)
//{
//	Coord aux;
//	aux.first = sprite.transform.x / CELL_WIDTH;
//	aux.second = (sprite.transform.y - GRID_Y_DISPLACEMENT) / CELL_HEIGHT;
//
//
//	ret.push_back(std::make_pair(aux, behavior));
//	for (int i = 0; i < size; i++) {
//		aux.first += direction;
//		if (aux.first == GRID_WIDTH || aux.first == -1) break;
//		ret.push_back(std::make_pair(aux, behavior));
//	}
//
//	return ret;
//}