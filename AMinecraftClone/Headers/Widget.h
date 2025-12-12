#pragma once
#include "glm/glm.hpp"

struct UIUVQuad {
	glm::vec2 uv0;
	glm::vec2 uv1;
	glm::vec2 uv2;
	glm::vec2 uv3;
};

class Widget
{
public:
	glm::vec2 position = glm::vec2(0.0, 0.0);
	glm::vec2 scale = glm::vec2(1.0, 1.0);
	float rotation = 0.0f;
};

