#pragma once
#include "MenuButtons.h"

class SavesMenu {
public:
	SavesMenu();

	void RenderScreen();
	void UpdateScreen();

	TextDisplayer titleText;
	MenuButtons CancelButton;
	MenuButtons DeleteWorldButton;
	MenuButtons WorldButtons[5]; //from world 1 - 5
	MenuButtons WorldType;

	bool IsDeleting = false;
};