#pragma once
#include "Element.hh"

class Log: public Element
{
	int timeToDestroy;

public:
	Log();
	Log(int row);
	~Log();

	bool ExitedMap();
};

