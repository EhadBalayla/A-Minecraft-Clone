#pragma once
#include "Widget.h"
#include "TextDisplayer.h"

class MenuButtons : public Widget
{
public:
	void Render();
	void Update();

	void(*Callback)();

	void SetText(const std::string& newText);
private:
	bool IsHovered = false;
	bool FirstClick = false;

	TextDisplayer text;
};

