#pragma once
#include "MenuButtons.h"

class PauseMenu
{
public:
	PauseMenu();

	void RenderScreen();
	void UpdateScreen();

	MenuButtons ContinueButton;
	MenuButtons QuitButton;
	MenuButtons OptionsButton;
};

