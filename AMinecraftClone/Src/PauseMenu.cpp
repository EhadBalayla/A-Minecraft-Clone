#include "PauseMenu.h"
#include "Game.h"

void Quit() {
	Game::ChangeState(GameState::MainMenu);
}

PauseMenu::PauseMenu() {
	ContinueButton.position = glm::vec2(640, 360);
	ContinueButton.scale = glm::vec2(600, 60);
	ContinueButton.SetText("Back to game");

	QuitButton.position = glm::vec2(640, 260);
	QuitButton.scale = glm::vec2(600, 60);
	QuitButton.SetText("Save and quit to title");
	QuitButton.Callback = Quit;

	OptionsButton.position = glm::vec2(640, 460);
	OptionsButton.scale = glm::vec2(600, 60);
	OptionsButton.SetText("Options...");
}


void PauseMenu::RenderScreen() {
	ContinueButton.Render();
	QuitButton.Render();
	OptionsButton.Render();
}
void PauseMenu::UpdateScreen() {
	ContinueButton.Update();
	QuitButton.Update();
	OptionsButton.Update();
}