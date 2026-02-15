#pragma once
#include "Widget.h"
#include "InventoryItem.h"
#include "TextDisplayer.h"

class ItemDisplayer : public Widget
{
public:
	InventoryItem* item;

	void Render();

	void SetTextProj(glm::mat4& MTX);
private:
	TextDisplayer countText;
};

