#pragma once
#include "ImageDisplayer.h"
#include "ItemDisplayer.h"

class PlayerInventory
{
public:
	PlayerInventory();

	void RenderScreen();
	void UpdateScreen();

	ItemDisplayer items[36];
	ImageDisplayer InventoryIMG;
};

