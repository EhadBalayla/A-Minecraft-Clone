#pragma once
#include "Widget.h"
#include <string>

class TextDisplayer : public Widget
{
public:

	void setText(const std::string& newText);
	void setTint(const glm::vec4& newTint);

	const std::string& GetText() const;

	void Render();
private:
	std::string Text;
	glm::vec4 Tint = glm::vec4(1.0f);
};

