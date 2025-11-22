#include "PauseMenu.h"
#include "MenuButtons.h"
#include "Game.h"

void Quit() {
	Game::ChangeState(GameState::MainMenu);
}

PauseMenu::PauseMenu() {
	//the "back to game" button
	std::unique_ptr<MenuButtons> ContinueButton = std::make_unique<MenuButtons>();
	ContinueButton.get()->SetPosition(glm::vec2(640, 360));
	ContinueButton.get()->SetScale(glm::vec2(600, 60));
	AddWidget(std::move(ContinueButton));

	//the "quit" button
	std::unique_ptr<MenuButtons> QuitButton = std::make_unique<MenuButtons>();
	QuitButton.get()->SetPosition(glm::vec2(640, 260));
	QuitButton.get()->SetScale(glm::vec2(600, 60));
	QuitButton.get()->Callback = Quit;
	AddWidget(std::move(QuitButton));
}