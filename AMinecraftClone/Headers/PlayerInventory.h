#pragma once
#include "UIScreen.h"
#include "ImageDisplayer.h"

class PlayerInventory : public UIScreen
{
public:
	PlayerInventory();

	void RenderScreen();
	void UpdateScreen();

	ImageDisplayer InventoryIMG;
};

