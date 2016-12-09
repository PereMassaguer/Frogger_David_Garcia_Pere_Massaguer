#pragma once
#include "Color.hh"
#include "ID.hh"
#include "InputManager.hh"
#include "Renderer.hh"
#include "Transform.hh"

class Button
{
	std::string text;
	Transform transform;
	Color foregroundColor, backgroundColor;
	std::pair<int, int> textSize;

public:
	Button(std::string && _text, Transform && _transform, Color && _foregroundColor, Color && _backgroundColor);
	~Button();

	void DrawButton();
	bool IsMoused(Transform transform);
};

