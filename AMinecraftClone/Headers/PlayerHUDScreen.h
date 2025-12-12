#pragma once
#include "UIScreen.h"
#include "ImageDisplayer.h"
#include "ItemDisplayer.h"
#include "TextDisplayer.h"

class PlayerHUDScreen : public UIScreen
{
public:
	PlayerHUDScreen(); //initializes the widgets for the player HUD, such as the reticle in the middle and hotbar

	void RenderScreen();
	void UpdateScreen();

	ItemDisplayer items[9];
	ImageDisplayer Hotbar;
	ImageDisplayer Reticle;
	ImageDisplayer Selection;
	TextDisplayer Text;
};

