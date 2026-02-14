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
	void SetDisabled(bool disabled);
	TextDisplayer text;
private:
	bool IsDisabled = false;
	bool IsHovered = false;

};

