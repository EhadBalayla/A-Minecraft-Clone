#include "MainMenuScreen.h"
#include "Game.h"

void StartGameCallback() {
	Game::ChangeState(GameState::InGame);
}

MainMenuScreen::MainMenuScreen() {
	Logo.uv = { glm::vec2(0.0, 0.0), glm::vec2(1.0, 0.0), glm::vec2(1.0, 1.0), glm::vec2(0.0, 1.0) };
	Logo.tex = &Game::logoTex;
	Logo.position = glm::vec2(640, 470);
	Logo.scale = glm::vec2(765, 755);

	SinglePlayerButton.position = glm::vec2(640, 360);
	SinglePlayerButton.scale = glm::vec2(600, 60);
	SinglePlayerButton.SetText("Singleplayer");
	SinglePlayerButton.Callback = StartGameCallback;

	MultiPlayerButton.position = glm::vec2(640, 430);
	MultiPlayerButton.scale = glm::vec2(600, 60);
	MultiPlayerButton.SetText("Multiplayer");

	TutorialButton.position = glm::vec2(640, 500);
	TutorialButton.scale = glm::vec2(600, 60);
	TutorialButton.SetText("Play tutorial level");

	OptionsButton.position = glm::vec2(640, 600);
	OptionsButton.scale = glm::vec2(600, 60);
	OptionsButton.SetText("Options...");
}


void MainMenuScreen::RenderScreen() {
	Logo.Render();
	SinglePlayerButton.Render();
	MultiPlayerButton.Render();
	TutorialButton.Render();
	OptionsButton.Render();
}
void MainMenuScreen::UpdateScreen() {
	SinglePlayerButton.Update();
	MultiPlayerButton.Update();
	TutorialButton.Update();
	OptionsButton.Update();
}