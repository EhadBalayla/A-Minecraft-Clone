#pragma once
#include "MenuButtons.h"

class OptionsMenu {
public:
	OptionsMenu();

	void RenderScreen();
	void UpdateScreen();

	MenuButtons DoneButton;
	MenuButtons LODCountButton;
	MenuButtons RenderDistanceButton;
};