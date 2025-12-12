#include "PauseMenu.h"
#include "Game.h"

void Quit() {
	Game::ChangeState(GameState::MainMenu);
}

PauseMenu::PauseMenu() {
	ContinueButton.position = glm::vec2(640, 360);
	ContinueButton.scale = glm::vec2(600, 60);

	QuitButton.position = glm::vec2(640, 260);
	QuitButton.scale = glm::vec2(600, 60);
	QuitButton.Callback = Quit;
}


void PauseMenu::RenderScreen() {
	ContinueButton.Render();
	QuitButton.Render();
}
void PauseMenu::UpdateScreen() {
	ContinueButton.Update();
	QuitButton.Update();
}