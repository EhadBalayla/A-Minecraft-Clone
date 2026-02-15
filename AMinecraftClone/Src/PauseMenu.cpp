#include "PauseMenu.h"
#include "Game.h"

void Quit() {
	Game::ChangeState(GameState::MainMenu);
}

void GoIntoOptions() {
	Game::InGameMenuState = InGameMenu::Options;
}
void BackToGame() {
	Game::player.IsPaused = false;
	SDL_ShowCursor(0);
	SDL_SetRelativeMouseMode(SDL_TRUE);
}

PauseMenu::PauseMenu() {
	ContinueButton.position = glm::vec2(0, 0);
	ContinueButton.scale = glm::vec2(600, 60);
	ContinueButton.SetText("Back to game");
	ContinueButton.Callback = BackToGame;
	ContinueButton.projMtx = &Game::ScrMidProj;
	ContinueButton.SetTextProj(Game::ScrMidProj);

	QuitButton.position = glm::vec2(0, -100);
	QuitButton.scale = glm::vec2(600, 60);
	QuitButton.SetText("Save and quit to title");
	QuitButton.Callback = Quit;
	QuitButton.projMtx = &Game::ScrMidProj;
	QuitButton.SetTextProj(Game::ScrMidProj);

	OptionsButton.position = glm::vec2(0, 100);
	OptionsButton.scale = glm::vec2(600, 60);
	OptionsButton.SetText("Options...");
	OptionsButton.Callback = GoIntoOptions;
	OptionsButton.projMtx = &Game::ScrMidProj;
	OptionsButton.SetTextProj(Game::ScrMidProj);
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