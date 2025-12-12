#pragma once
#include "UIScreen.h"
#include "MenuButtons.h"
#include "ImageDisplayer.h"

class MainMenuScreen : public UIScreen {
public:
	MainMenuScreen();

	void RenderScreen();
	void UpdateScreen();

	ImageDisplayer Logo;
	MenuButtons SinglePlayerButton;
	MenuButtons MultiPlayerButton;
	MenuButtons TutorialButton;
	MenuButtons OptionsButton;
};