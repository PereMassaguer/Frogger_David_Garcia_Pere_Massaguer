#include "Element.hh"



Element::Element()
{
}

Element::Element(int row, int lenght)
{
	sprite.objectID = ObjectID::FrogIUp;

	sprite.transform = Transform(row % 2 == 1 ? 0 : W.GetWidth(), GRID_Y_DISPLACEMENT, CELL_WIDTH, CELL_HEIGHT);
	direction = row % 2 == 1 ? 1 : -1;
	acumulateX = 0.0f;
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

