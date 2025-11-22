#include "MainMenuScreen.h"
#include "MenuButtons.h"
#include "Game.h"

void StartGameCallback() {
	Game::ChangeState(GameState::InGame);
}

MainMenuScreen::MainMenuScreen() {
	//the "Minecraft" logo
	UIUVQuad logoUV = { glm::vec2(0.0, 0.0), glm::vec2(1.0, 0.0), glm::vec2(1.0, 1.0), glm::vec2(0.0, 1.0) };
	std::unique_ptr<Widget> logo = std::make_unique<Widget>(logoUV);
	logo.get()->AssignTexture(Game::e_LoadedTextures[6]);
	logo.get()->SetPosition(glm::vec2(640, 470));
	logo.get()->SetScale(glm::vec2(765, 755));
	AddWidget(std::move(logo));





	//the "Single Player" button
	std::unique_ptr<MenuButtons> SinglePlayer = std::make_unique<MenuButtons>();
	SinglePlayer.get()->SetPosition(glm::vec2(640, 360));
	SinglePlayer.get()->SetScale(glm::vec2(600, 60));
	SinglePlayer.get()->Callback = StartGameCallback;
	AddWidget(std::move(SinglePlayer));

	//the "Single Player" button
	std::unique_ptr<MenuButtons> MultiPlayer = std::make_unique<MenuButtons>();
	MultiPlayer.get()->SetPosition(glm::vec2(640, 430));
	MultiPlayer.get()->SetScale(glm::vec2(600, 60));
	AddWidget(std::move(MultiPlayer));

	//the "Single Player" button
	std::unique_ptr<MenuButtons> PlayTutorial = std::make_unique<MenuButtons>();
	PlayTutorial.get()->SetPosition(glm::vec2(640, 500));
	PlayTutorial.get()->SetScale(glm::vec2(600, 60));
	AddWidget(std::move(PlayTutorial));

	//the "Options" button
	std::unique_ptr<MenuButtons> OptionsButton = std::make_unique<MenuButtons>();
	OptionsButton.get()->SetPosition(glm::vec2(640, 600));
	OptionsButton.get()->SetScale(glm::vec2(600, 60));
	AddWidget(std::move(OptionsButton));
}