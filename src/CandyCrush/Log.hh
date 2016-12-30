#pragma once
#include "Element.hh"

class Log: public Element
{
public:
	Log();
	Log(int i);
	~Log();

	bool ExitedMap();
};

