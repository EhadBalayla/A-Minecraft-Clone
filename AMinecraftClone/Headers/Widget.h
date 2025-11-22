#pragma once
#include "glm/glm.hpp"
#include "Shader.h"
#include "Texture.h"
#include <vector>

struct UIUVQuad {
	glm::vec2 uv0;
	glm::vec2 uv1;
	glm::vec2 uv2;
	glm::vec2 uv3;
};

class Widget
{
public:
	glm::vec2 position = glm::vec2(0.0, 0.0); //position on the screen relative to the middle
	glm::vec2 scale = glm::vec2(1.0, 1.0); //scale on the screen relative to the sprite's middle
	float rotation = 0.0f; //rotation on the screen relative to the sprite's middle
	
	Texture* m_Texture = nullptr; //texture that a widget will display
	UIUVQuad uvs;

	Widget();
	Widget(UIUVQuad uvs);
	~Widget();
	virtual void RenderWidget(Shader& shader);
	virtual void UpdateWidget();
	void AssignTexture(Texture* texture); //loads the texture into the widget
	
	void SetPosition(glm::vec2 newPosition);
	void SetRotation(float newRot);
	void SetScale(glm::vec2 newScale);

	glm::vec2 GetPosition();
	float GetRotation();
	glm::vec2 GetScale();
};

