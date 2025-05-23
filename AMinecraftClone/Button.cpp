#include "Button.h"
#include "SDL.h"
#include <iostream>

Button::Button(UIUVQuad offUV, UIUVQuad normalUV, UIUVQuad hoveredUV) : Widget(normalUV){
	m_OffUV = offUV;
	m_NormalUV = normalUV;
	m_HoveredUV = hoveredUV;


}

void Button::UpdateWidget() {
	int MouseX = 0;
	int MouseY = 0;
	auto mouseState = SDL_GetMouseState(&MouseX, &MouseY);

	//std::cout << "MouseX: " << MouseX << ", MouseY: " << MouseY << std::endl;

	if ((MouseX > (int)GetPosition().x - ((int)GetScale().x / 2) && MouseX < (int)GetPosition().x + ((int)GetScale().x) / 2) && //check if the mouseX is in bounds of the button widget
		(MouseY > (int)GetPosition().y - ((int)GetScale().y / 2) && MouseY < (int)GetPosition().y + ((int)GetScale().y) / 2)) { //check if the MouseY is in bounds of the button
		if (!IsHovered) { //if wasn't hovered already then make hovered
			IsHovered = true;
			UpdateButtonUVs(m_HoveredUV);
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
	verticies[0] = { glm::vec2(-0.5, -0.5), uvs.uv0 }; //bottom left
	verticies[1] = { glm::vec2(0.5, -0.5), uvs.uv1 }; //bottom right
	verticies[2] = { glm::vec2(0.5, 0.5), uvs.uv2 }; // top right
	verticies[3] = { glm::vec2(-0.5, 0.5), uvs.uv3 };// top left

	MakeData();
}