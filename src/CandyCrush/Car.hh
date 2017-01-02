#pragma once
#include "Element.hh"

class Car: public Element
{
public:
	Car();
	Car(int row);
	~Car();

	bool ExitedMap();
};

