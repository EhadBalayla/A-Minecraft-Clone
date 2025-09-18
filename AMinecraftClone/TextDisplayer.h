#pragma once
#include "Widget.h"

class TextDisplayer : public Widget
{
public:
	void setText(const std::string& newText);
private:
	std::string text;
};

