#pragma once
#include "Color.hh"
#include "ID.hh"
#include "InputManager.hh"
#include "Renderer.hh"
#include "SceneManager.hh"
#include "Transform.hh"

class Button
{
	std::string text;
	Transform transform;
	Color textColor, backgroundColor;
	std::pair<int, int> textSize;


	typedef void(*eventFunction)();
	eventFunction buttonVoidBehavior;
public:
	Button();
	Button(std::string && _text, Transform && _transform, Color && _backgroundColor);
	~Button();

	void DrawButton();
	bool IsMoused();
	void SetButtonBehavior(eventFunction buttonVoid);
	void ExecuteBehavior();
};

