#pragma once
#include "Element.hh"

class Log: public Element
{
public:
	Log();
	Log(int row);
	~Log();

	bool ExitedMap();
};

