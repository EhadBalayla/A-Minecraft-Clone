#pragma once
#include "UIScreen.h"
#include "MenuButtons.h"

class PauseMenu : public UIScreen
{
public:
	PauseMenu();

	void RenderScreen();
	void UpdateScreen();

	MenuButtons ContinueButton;
	MenuButtons QuitButton;
};

