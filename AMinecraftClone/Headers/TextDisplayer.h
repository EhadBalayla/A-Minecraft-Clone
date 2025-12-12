#pragma once
#include "Widget.h"
#include <string>
#include <vector>

struct Letter {
	UIUVQuad uvs;
	glm::vec2 size;
	float advance;
	float height;
};

class TextDisplayer : public Widget
{
public:

	void setText(const std::string& newText);

	void Render();
private:
	std::vector<Letter> CharUVs;
};

