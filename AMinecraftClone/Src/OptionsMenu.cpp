#include "OptionsMenu.h"
#include "Game.h"

void DoneButtonCallback() {
	if (Game::GetGameState() == GameState::MainMenu) Game::MainMenuState = MainMenuMenu::Title;
	else Game::InGameMenuState = InGameMenu::PauseMenu;
}

void LODCountButtonCallback() {
	if (Game::LODCount >= 5) Game::LODCount = 1;
	else Game::LODCount++;

	Game::optionsMenu.LODCountButton.SetText("LOD Count: " + std::to_string(Game::LODCount));
}

void RenderDistanceButtonCallback() {
	if (Game::RenderDistance >= 32) Game::RenderDistance = 3;
	else Game::RenderDistance++;

	Game::optionsMenu.RenderDistanceButton.SetText("Render Distance: " + std::to_string(Game::RenderDistance));
}

OptionsMenu::OptionsMenu() {
	DoneButton.position = glm::vec2(640, 655);
	DoneButton.scale = glm::vec2(600, 60);
	DoneButton.SetText("Done");
	DoneButton.Callback = DoneButtonCallback;

	LODCountButton.position = glm::vec2(640, 100);
	LODCountButton.scale = glm::vec2(600, 60);
	LODCountButton.SetText("LOD Count: " + std::to_string(Game::LODCount));
	LODCountButton.Callback = LODCountButtonCallback;

	RenderDistanceButton.position = glm::vec2(640, 180);
	RenderDistanceButton.scale = glm::vec2(600, 60);
	RenderDistanceButton.SetText("Render Distance: " + std::to_string(Game::RenderDistance));
	RenderDistanceButton.Callback = RenderDistanceButtonCallback;
}

void OptionsMenu::RenderScreen() {
	DoneButton.Render();
	LODCountButton.Render();
	RenderDistanceButton.Render();
}
void OptionsMenu::UpdateScreen() {
	DoneButton.Update();
	LODCountButton.Update();
	RenderDistanceButton.Update();
}