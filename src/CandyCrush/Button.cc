#include "Button.hh"


Button::Button()
{
}

Button::Button(std::string && _text, Transform && _transform, Color && _backgroundColor)
:text(_text), transform(_transform), backgroundColor(_backgroundColor) {
		
	TTF_SizeText(R.GetFont<FontID::ARIAL>(), text.c_str(), &textSize.first, &textSize.second);
}

Button::~Button()
{

}

void Button::DrawButton()
{
	if (IsMoused()) {
		R.Push(TTF_RenderText_Shaded(R.GetFont<FontID::ARIAL>(), text.c_str(), BLACK, backgroundColor()), transform);
	}
	else {
		R.Push(TTF_RenderText_Shaded(R.GetFont<FontID::ARIAL>(), text.c_str(), backgroundColor(), BLACK), transform);
	}
}

bool Button::IsMoused() {
	MouseCoords mouseCoords = IM.GetMouseCoords();
	return (transform.x - textSize.first / 2) < mouseCoords.x 
		&& (transform.x + textSize.first / 2) > mouseCoords.x 
		&& (transform.y - textSize.second / 2) < mouseCoords.y 
		&& (transform.y + textSize.second / 2) > mouseCoords.y;
}

void Button::SetButtonBehavior(eventFunction buttonVoid)
{
	buttonVoidBehavior = buttonVoid;
}


void Button::ExecuteBehavior()
{
	buttonVoidBehavior();
}

