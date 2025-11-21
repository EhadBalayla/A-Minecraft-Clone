#include "Button.h"
#include "SDL.h"

#include "Game.h"

#include <iostream>

Button::Button(UIUVQuad offUV, UIUVQuad normalUV, UIUVQuad hoveredUV) : Widget(normalUV){
	m_OffUV = offUV;
	m_NormalUV = normalUV;
	m_HoveredUV = hoveredUV;

	UpdateButtonUVs(uvs);
}

void Button::UpdateWidget() {
	int MouseX = 0;
	int MouseY = 0;
	auto mouseState = SDL_GetMouseState(&MouseX, &MouseY);

	if ((MouseX > (int)GetPosition().x - ((int)GetScale().x / 2) && MouseX < (int)GetPosition().x + ((int)GetScale().x) / 2) && //check if the mouseX is in bounds of the button widget
		(MouseY > (int)GetPosition().y - ((int)GetScale().y / 2) && MouseY < (int)GetPosition().y + ((int)GetScale().y) / 2)) { //check if the MouseY is in bounds of the button
		if (!IsHovered) { //if wasn't hovered already then make hovered
			IsHovered = true;
			UpdateButtonUVs(m_HoveredUV);
		}

		if (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) {
			if (!FirstClick) {
				FirstClick = true;
				Callback();
			}
		}
		else {
			if (FirstClick)
				FirstClick = false;
		}
	}
	else {
		if (IsHovered) {
			IsHovered = false;
			UpdateButtonUVs(m_NormalUV);
		}
	}
}

void Button::UpdateButtonUVs(UIUVQuad uvs) {
	this->uvs = uvs;
}