#pragma once
#include "Widget.h"

class MenuButtons : public Widget
{
public:
	void Render();
	void Update();

	void(*Callback)();
private:
	bool IsHovered = false;
	bool FirstClick = false;
};

