#pragma once
#include "Widget.h"
#include "InventoryItem.h"

class ItemDisplayer : public Widget
{
public:
	InventoryItem* item;

	void Render();
private:
};

