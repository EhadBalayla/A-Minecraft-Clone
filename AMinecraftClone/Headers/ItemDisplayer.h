#pragma once
#include "Widget.h"
#include "InventoryItem.h"
#include "TextDisplayer.h"

class ItemDisplayer : public Widget
{
public:
	InventoryItem* item;

	void Render();
	void Update(); //specifically for when i want to make the displayer a clickable object

	void SetTextProj(glm::mat4& MTX);
private:
	TextDisplayer countText;
	bool IsHovered = false;
};

