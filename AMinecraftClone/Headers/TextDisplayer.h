#pragma once
#include "Widget.h"

struct Letter {
	UIUVQuad uvs;
	glm::vec2 size;
	float advance;
	float height;
};

class TextDisplayer : public Widget
{
public:
	TextDisplayer();

	void setText(const std::string& newText);

	void RenderWidget(Shader& shader) override;
private:
	std::vector<Letter> CharUVs;
};

