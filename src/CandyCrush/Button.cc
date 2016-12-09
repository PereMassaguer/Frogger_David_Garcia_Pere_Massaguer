#include "Button.hh"



Button::Button(std::string && _text, Transform && _transform, Color && _foregroundColor, Color && _backgroundColor)
:text(_text), transform(_transform), foregroundColor(_foregroundColor), backgroundColor(_backgroundColor) {

	TTF_SizeText(R.GetFont<FontID::ARIAL>(), text.c_str(), &textSize.first, &textSize.second);

}


Button::~Button()
{
}

void Button::DrawButton()
{
	if (IsMoused(transform)) {

		R.Push(TTF_RenderText_Shaded(R.GetFont<FontID::ARIAL>(), text.c_str(), foregroundColor(), backgroundColor()), transform);
	}
	else
		R.Push(TTF_RenderText_Shaded(R.GetFont<FontID::ARIAL>(), text.c_str(), backgroundColor(), foregroundColor()), transform);

}

bool Button::IsMoused(Transform transform) {
	MouseCoords mouseCoords = IM.GetMouseCoords();
	return (transform.x - textSize.first / 2) < mouseCoords.x 
		&& (transform.x + textSize.first / 2) > mouseCoords.x 
		&& (transform.y - textSize.second / 2) < mouseCoords.y 
		&& (transform.y + textSize.second / 2) > mouseCoords.y;
}